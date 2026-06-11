# Image meta information
string image_name
int32 height
int32 width

# Keypoints (n keypoints)
# Each keypoint has: x, y, size, angle
float32[] keypoints_x
float32[] keypoints_y  
float32[] keypoints_size
float32[] keypoints_angle

# Descriptors flattened: n x 128 floats
# Row-major order: descriptor_0[0..127], descriptor_1[0..127], ...
float32[] descriptors

# Number of keypoints
int32 num_keypoints

# Publish timestamp (seconds since epoch, set right before publishing)
# Used by the subscriber to measure transport latency
float64 publish_timestamp
