import cv2
import numpy as np
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import time

# ==================== TIMERS ====================
t_start = time.perf_counter()

# Load images
t0 = time.perf_counter()
large = cv2.imread('ankara.jpg')
small = cv2.imread('test.jpg')
t_load = time.perf_counter() - t0

if large is None or small is None:
    print("Error: Could not load one or both images.")
    exit(1)

# Convert to grayscale
large_gray = cv2.cvtColor(large, cv2.COLOR_BGR2GRAY)
small_gray = cv2.cvtColor(small, cv2.COLOR_BGR2GRAY)

# Initialize SIFT
sift = cv2.SIFT_create()

# Detect keypoints and compute descriptors
t1 = time.perf_counter()
kp_large, des_large = sift.detectAndCompute(large_gray, None)
kp_small, des_small = sift.detectAndCompute(small_gray, None)
t_sift = time.perf_counter() - t1

print(f"Keypoints in large image: {len(kp_large)}")
print(f"Keypoints in small image: {len(kp_small)}")

# FLANN parameters for SIFT
FLANN_INDEX_KDTREE = 1
index_params = dict(algorithm=FLANN_INDEX_KDTREE, trees=5)
search_params = dict(checks=50)

flann = cv2.FlannBasedMatcher(index_params, search_params)

# Match descriptors
t2 = time.perf_counter()
matches = flann.knnMatch(des_small, des_large, k=2)
t_match = time.perf_counter() - t2

# Lowe's ratio test
t3 = time.perf_counter()
good_matches = []
for m, n in matches:
    if m.distance < 0.7 * n.distance:
        good_matches.append(m)
t_ratio = time.perf_counter() - t3

print(f"Good matches: {len(good_matches)}")

if len(good_matches) < 4:
    print("Not enough good matches to compute homography.")
else:
    # Extract matched points
    src_pts = np.float32([kp_small[m.queryIdx].pt for m in good_matches]).reshape(-1, 1, 2)
    dst_pts = np.float32([kp_large[m.trainIdx].pt for m in good_matches]).reshape(-1, 1, 2)

    # Find homography
    t4 = time.perf_counter()
    M, mask = cv2.findHomography(src_pts, dst_pts, cv2.RANSAC, 5.0)
    t_homo = time.perf_counter() - t4
    matches_mask = mask.ravel().tolist()

    h, w = small_gray.shape
    pts = np.float32([[0, 0], [0, h - 1], [w - 1, h - 1], [w - 1, 0]]).reshape(-1, 1, 2)
    dst = cv2.perspectiveTransform(pts, M)

    # Draw the match region on the large image
    large_with_box = large.copy()
    cv2.polylines(large_with_box, [np.int32(dst)], True, (0, 255, 0), 3, cv2.LINE_AA)

    # Draw matches
    draw_params = dict(
        matchColor=(0, 255, 0),
        singlePointColor=None,
        matchesMask=matches_mask,
        flags=cv2.DrawMatchesFlags_NOT_DRAW_SINGLE_POINTS,
    )
    img_matches = cv2.drawMatches(small, kp_small, large, kp_large, good_matches, None, **draw_params)

    t5 = time.perf_counter()

    # Build result figure
    plt.figure(figsize=(20, 10))

    plt.subplot(221)
    plt.imshow(cv2.cvtColor(small, cv2.COLOR_BGR2RGB))
    plt.title("Query Image (test.jpg)")
    plt.axis("off")

    plt.subplot(222)
    plt.imshow(cv2.cvtColor(large, cv2.COLOR_BGR2RGB))
    plt.title("Train Image (ankara.jpg)")
    plt.axis("off")

    plt.subplot(223)
    plt.imshow(cv2.cvtColor(large_with_box, cv2.COLOR_BGR2RGB))
    plt.title("Detected Region on Large Image")
    plt.axis("off")

    plt.subplot(224)
    plt.imshow(cv2.cvtColor(img_matches, cv2.COLOR_BGR2RGB))
    plt.title("SIFT Feature Matches")
    plt.axis("off")

    plt.tight_layout()
    plt.savefig("matching_result.png", dpi=150)
    t_draw = time.perf_counter() - t5

    t_total = time.perf_counter() - t_start

    # ==================== PERFORMANCE METRICS ====================
    print("\n" + "=" * 50)
    print("PERFORMANCE METRICS")
    print("=" * 50)
    print(f"Image loading:         {t_load*1000:8.1f} ms")
    print(f"SIFT detection:        {t_sift*1000:8.1f} ms  ({len(kp_large) + len(kp_small)} keypoints)")
    print(f"FLANN matching:        {t_match*1000:8.1f} ms  ({len(matches)} raw matches)")
    print(f"Lowe's ratio test:     {t_ratio*1000:8.1f} ms  ({len(good_matches)} good matches)")
    print(f"Homography (RANSAC):   {t_homo*1000:8.1f} ms")
    print(f"Drawing + saving:      {t_draw*1000:8.1f} ms")
    print("-" * 50)
    print(f"Total execution:       {t_total*1000:8.1f} ms")
    print("=" * 50)
    print("Result saved to matching_result.png")

    # Also print matching region coordinates
    print("\nMatching region corners on large image:")
    for i, pt in enumerate(dst.reshape(4, 2)):
        print(f"  Corner {i + 1}: ({pt[0]:.1f}, {pt[1]:.1f})")