# Sequential Baseline Implementation — Detaylı Açıklama

## İçindekiler
1. [Sistem Genel Bakışı](#1-sistem-genel-bakışı)
2. [Mimari Diyagram](#2-mimari-diyagram)
3. [ROS2 Paket Yapısı](#3-ros2-paket-yapısı)
4. [Özel Mesaj Tanımı: ImageFeatures](#4-özel-mesaj-tanımı-imagefeatures)
5. [Düğüm 1: sift_publisher](#5-düğüm-1-sift_publisher)
6. [Düğüm 2: flann_subscriber](#6-düğüm-2-flann_subscriber)
7. [Performans Ölçüm Altyapısı: PerfTracker](#7-performans-ölçüm-altyapısı-perftracker)
8. [Pipeline Akış Şeması (Sıralı)](#8-pipeline-akış-şeması-sıralı)
9. [Pipeline Çıktıları (Görsel)](#9-pipeline-çıktıları-görsel)
10. [Performans Tablosu ve Yorumlama](#10-performans-tablosu-ve-yorumlama)
11. [Sequential vs Paralel Karşılaştırması](#11-sequential-vs-paralel-karşılaştırması)

---

## 1. Sistem Genel Bakışı

Bu implementasyon, **görüntü eşleme (image matching)** problemini çözen **sıralı (sequential/tek iş parçacıklı)** bir ROS2 tabanlı sistemdir. Sistem şu işlemleri sırayla gerçekleştirir:

```
Girdiler:                        Süreç:                             Çıktılar:
┌──────────┐          ┌─────────────────────┐          ┌─────────────────────┐
│ankara.jpg│─────────►│ 1. Görüntü Yükleme  │          │matching_result.png  │
│(büyük)   │          └──────────┬──────────┘          │(görselleştirme)     │
└──────────┘                     ▼                      └─────────────────────┘
                         ┌─────────────────────┐        ┌─────────────────────┐
┌──────────┐             │ 2. Gri Tonlama      │        │publisher_perf.txt   │
│test.jpg  │─────────►   │    (BGR → Gray)     │        │(yayıncı performans) │
│(query)   │             └──────────┬──────────┘        └─────────────────────┘
└──────────┘                        ▼                    ┌─────────────────────┐
                            ┌─────────────────────┐      │subscriber_perf.txt  │
                            │ 3. SIFT Çıkarımı    │      │(abone performans)   │
                            │    (detectAndCompute)│      └─────────────────────┘
                            └──────────┬──────────┘      ┌─────────────────────┐
                                       ▼                 │pipeline_perf.json   │
                            ┌─────────────────────┐      │(birleşik rapor)     │
                            │ 4. ROS2 Mesajlarına  │      └─────────────────────┘
                            │    Dönüştürme        │
                            └──────────┬──────────┘
                                       ▼
                            ┌─────────────────────┐
                            │ 5. Yayınlama        │
                            │    (publish)        │
                            └──────────┬──────────┘
                                       ▼
                            ┌─────────────────────┐
                            │ 6. FLANN Eşleme     │
                            │    (knnMatch)       │
                            └──────────┬──────────┘
                                       ▼
                            ┌─────────────────────┐
                            │ 7. Lowe's Ratio     │
                            │    Test (filtreleme) │
                            └──────────┬──────────┘
                                       ▼
                            ┌─────────────────────┐          ┌──────────────────────┐
                            │ 8. RANSAC Homografi│───►       │ Köşe noktaları:     │
                            │    (findHomography) │           │ (x1,y1), (x2,y2)... │
                            └──────────┬──────────┘           └──────────────────────┘
                                       ▼
                            ┌─────────────────────┐
                            │ 9. Görselleştirme   │
                            │    (matplotlib)     │
                            └─────────────────────┘
```

**Temel karakteristik:** Tüm işlemler **tek bir CPU çekirdeğinde sıralı olarak** çalışır. Paralel versiyonla karşılaştırma yapmak için **baseline (referans)** görevi görür.

---

## 2. Mimari Diyagram

```
┌────────────────────────────────────────────────────────────────────────────────────┐
│                            ROS2 DÜĞÜM MİMARİSİ                                      │
│                                                                                     │
│  ┌─────────────────────────────────────┐          ┌────────────────────────────────┐│
│  │         sift_publisher              │          │      flann_subscriber           ││
│  │         (Node)                      │          │      (Node)                    ││
│  │                                     │          │                                ││
│  │  ┌───────────────────────────┐      │          │  ┌────────────────────────┐    ││
│  │  │ 1_image_loading           │      │          │  │ received_large_features│    ││
│  │  │ cv2.imread()              │      │          │  │ (callback)              │    ││
│  │  └──────────┬────────────────┘      │          │  └──────────┬─────────────┘    ││
│  │             ▼                      │          │             ▼                  ││
│  │  ┌───────────────────────────┐      │          │  ┌────────────────────────┐    ││
│  │  │ 2_grayscale_conversion    │      │          │  │ received_query_features│    ││
│  │  │ cv2.cvtColor(BGR→GRAY)    │      │          │  │ (callback)              │    ││
│  │  └──────────┬────────────────┘      │          │  └──────────┬─────────────┘    ││
│  │             ▼                      │          │             ▼                  ││
│  │  ┌───────────────────────────┐      │          │  ┌────────────────────────┐    ││
│  │  │ 3_sift_large_image        │      │          │  │ 1_data_reconstruction  │    ││
│  │  │ detectAndCompute(large)   │──────┼──────────┼─►│ _to_keypoints()        │    ││
│  │  └──────────┬────────────────┘      │          │  │ _to_descriptors()      │    ││
│  │             ▼                      │          │  └──────────┬─────────────┘    ││
│  │  ┌───────────────────────────┐      │  ROS2    │             ▼                  ││
│  │  │ 4_sift_query_image        │      │  Topics  │  ┌────────────────────────┐    ││
│  │  │ detectAndCompute(query)   │──────┼──────────┼─►│ 2_flann_index_build    │    ││
│  │  └──────────┬────────────────┘      │ /sift/   │  │ FlannBasedMatcher()    │    ││
│  │             ▼                      │ large_   │  └──────────┬─────────────┘    ││
│  │  ┌───────────────────────────┐      │ features │             ▼                  ││
│  │  │ 5_message_serialization   │      │          │  ┌────────────────────────┐    ││
│  │  │ _build_feature_msg()      │──────┼──────────┼─►│ 3_flann_knn_matching   │    ││
│  │  └──────────┬────────────────┘      │ /sift/   │  │ knnMatch(k=2)          │    ││
│  │             ▼                      │ query_   │  └──────────┬─────────────┘    ││
│  │  ┌───────────────────────────┐      │ features │             ▼                  ││
│  │  │ 6_publish_messages        │──────┼──────────┼─►│ 4_lowes_ratio_test     │    ││
│  │  │ publisher.publish()       │      │          │  │ m.dist < 0.7 * n.dist  │    ││
│  │  └───────────────────────────┘      │          │  └──────────┬─────────────┘    ││
│  │                                     │          │             ▼                  ││
│  └─────────────────────────────────────┘          │  ┌────────────────────────┐    ││
│                                                    │  │ 5_point_extraction    │    ││
│  Publisher tarafı:                                 │  │ src_pts / dst_pts     │    ││
│  • Görüntüleri yükle                              │  └──────────┬─────────────┘    ││
│  • SIFT çıkar                                     │             ▼                  ││
│  • Mesaj oluştur                                  │  ┌────────────────────────┐    ││
│  • 0.5s bekle → yayınla                            │  │ 6_homography_ransac   │    ││
│  • 2s bekle → kapan                               │  │ findHomography(RANSAC) │    ││
│                                                    │  └──────────┬─────────────┘    ││
│                                                    │             ▼                  ││
│                                                    │  ┌────────────────────────┐    ││
│                                                    │  │ 7_perspective_transform│    ││
│                                                    │  │ perspectiveTransform() │    ││
│                                                    │  └──────────┬─────────────┘    ││
│                                                    │             ▼                  ││
│                                                    │  ┌────────────────────────┐    ││
│                                                    │  │ 8_final_composite      │    ││
│                                                    │  │ matplotlib 4-panel    │    ││
│                                                    │  └────────────────────────┘    ││
│                                                    │                                ││
│                                                    │  Subscriber tarafı:            ││
│                                                    │  • Mesajları bekle (timer)     ││
│                                                    │  • Her aşamayı zamanla         ││
│                                                    │  • Performans raporu oluştur  ││
│                                                    │  • Kapan                       ││
│                                                    └────────────────────────────────┘│
└────────────────────────────────────────────────────────────────────────────────────┘
```

**İletişim modeli:** Publisher → Subscriber (asenkron, ROS2 topic üzerinden)

- `sift_publisher` mesajları yayınlar ve kapanır
- `flann_subscriber` her iki mesajı alana kadar bekler (100ms periyodik timer), sonra işleme başlar

---

## 3. ROS2 Paket Yapısı

```
src/sequential/                      # ROS2 paketi
│
├── sequential/                      # Python modülü
│   ├── __init__.py                  # Boş init dosyası
│   ├── sift_publisher.py            # SIFT yayıncı düğümü (216 satır)
│   ├── flann_subscriber.py          # FLANN abone düğümü (422 satır)
│   └── perf_utils.py               # Performans ölçüm araçları (206 satır)
│
├── launch/
│   └── sift_matching.launch.py      # Her iki düğümü başlatan launch dosyası (25 satır)
│
├── output/                          # Çalışma zamanı çıktıları
│   ├── publisher_1_large_grayscale.png
│   ├── publisher_2_query_grayscale.png
│   ├── publisher_3_large_keypoints.png
│   ├── publisher_4_query_keypoints.png
│   ├── subscriber_1_large_received_keypoints.png
│   ├── subscriber_2_query_received_keypoints.png
│   ├── subscriber_3_ratio_test_matches.png
│   ├── subscriber_4_ransac_inlier_matches.png
│   ├── subscriber_5_detected_region.png
│   ├── subscriber_6_matching_result.png
│   ├── publisher_performance.{json,txt}
│   ├── subscriber_performance.{json,txt}
│   └── pipeline_performance.json
│
├── resource/
│   └── sequential                   # ROS2 resource işaretçisi
│
├── setup.py                         # Paket yapılandırması
├── setup.cfg                        # Script yolları
├── package.xml                      # Bağımlılıklar
└── WALKTHROUGH.md                   # Kullanım kılavuzu
```

### setup.py — Giriş Noktaları

```python
entry_points={
    'console_scripts': [
        'sift_publisher = sequential.sift_publisher:main',
        'flann_subscriber = sequential.flann_subscriber:main',
    ],
},
```

Her iki düğüm de ROS2 `console_scripts` olarak kayıtlıdır:
- `ros2 run sequential sift_publisher`
- `ros2 run sequential flann_subscriber`

### package.xml — Bağımlılıklar

| Bağımlılık | Tür | Açıklama |
|---|---|---|
| `rclpy` | exec_depend | ROS2 Python istemci kütüphanesi |
| `std_msgs` | exec_depend | Standart ROS2 mesaj tipleri |
| `sift_interfaces` | exec_depend | Özel `ImageFeatures.msg` tanımını içeren paket |
| `cv_bridge` | exec_depend | ROS ↔ OpenCV görüntü dönüşümü |
| `sensor_msgs` | exec_depend | Sensör verisi mesajları (görüntü) |
| `ament_copyright` | test_depend | Lisans başlık testleri |
| `ament_flake8` | test_depend | Python kod stili denetleyicisi |
| `ament_pep257` | test_depend | Docstring stili denetleyicisi |
| `python3-pytest` | test_depend | Python test framework'ü |

---

## 4. Özel Mesaj Tanımı: ImageFeatures

`src/sift_interfaces/msg/ImageFeatures.msg` dosyasında tanımlanmıştır:

```
# ImageFeatures.msg
string image_name          # Görüntü dosya adı (ör: "ankara.jpg")
int32 height               # Görüntü yüksekliği (piksel)
int32 width                # Görüntü genişliği (piksel)
int32 num_keypoints        # Anahtar nokta sayısı
float32[] keypoints_x      # X koordinatları (num_keypoints adet)
float32[] keypoints_y      # Y koordinatları (num_keypoints adet)
float32[] keypoints_size   # Anahtar nokta boyutları (num_keypoints adet)
float32[] keypoints_angle  # Anahtar nokta açıları (num_keypoints adet)
float32[] descriptors      # 128-boyutlu SIFT tanımlayıcıları (düzleştirilmiş)
```

### Serileştirme Şeması

```
ImageFeatures Mesaj Yapısı (Bellek Düzeni):
┌─────────────────────────────────────────────────┐
│ image_name  (string)                            │
├─────────────────────────────────────────────────┤
│ height       │ width        │ num_keypoints     │
│ (int32)      │ (int32)      │ (int32)           │
├─────────────────────────────────────────────────┤
│ keypoints_x[]  (float32[])                      │
│ keypoints_y[]  (float32[])                      │
│ keypoints_size[]  (float32[])                   │
│ keypoints_angle[]  (float32[])                  │
├─────────────────────────────────────────────────┤
│ descriptors[]  (float32[])                      │
│ (num_keypoints × 128 adet)                      │
└─────────────────────────────────────────────────┘
```

### Bellek Kullanımı Hesaplaması

```
Bir anahtar nokta için gereken alan:
  keypoints_x  : 1 × 4 bayt =  4 bayt
  keypoints_y  : 1 × 4 bayt =  4 bayt
  keypoints_size : 1 × 4 bayt =  4 bayt
  keypoints_angle: 1 × 4 bayt =  4 bayt
  Toplam       :               16 bayt / anahtar nokta

Bir tanımlayıcı için gereken alan:
  128 × float32 = 128 × 4 bayt = 512 bayt / tanımlayıcı

Örnek: 6200 anahtar noktalı büyük görüntü (ankara.jpg):
  - Anahtar nokta verisi: 6200 × 16 bayt ≈ 99 KB
  - Tanımlayıcı verisi: 6200 × 512 bayt ≈ 3.1 MB
  - Toplam payload: ≈ 3.2 MB

Örnek: 2300 anahtar noktalı query görüntüsü (test.jpg):
  - Anahtar nokta verisi: 2300 × 16 bayt ≈ 37 KB
  - Tanımlayıcı verisi: 2300 × 512 bayt ≈ 1.2 MB
  - Toplam payload: ≈ 1.2 MB
```

---

## 5. Düğüm 1: sift_publisher

### 5.1 Yaşam Döngüsü ve Aşamalar

```
Zaman Çizelgesi (ms olarak, örnek değerler):
 0 ms ─── Node.__init__() başlar
       │
 5 ms ─── 1_image_loading: cv2.imread() × 2
       │   (12.3 ms)
       │
15 ms ─── 2_grayscale_conversion: cv2.cvtColor() × 2
       │   (2.1 ms)
       │
20 ms ─── 2b_save_grayscale_images: cv2.imwrite() × 2
       │   (5.0 ms)
       │
25 ms ─── 3_sift_large_image: detectAndCompute(large)
       │   (245.0 ms, 6200 keypoints)
       │
270 ms ─── 4_sift_query_image: detectAndCompute(query)
       │   (100.1 ms, 2300 keypoints)
       │
370 ms ─── 4b_save_keypoint_images: drawKeypoints + imwrite
       │   (15.0 ms)
       │
385 ms ─── 5_message_serialization: _build_feature_msg() × 2
       │   (8.5 ms)
       │
393 ms ─── [0.5s bekleme] create_timer(0.5, _publish_and_shutdown)
       │
893 ms ─── 6_publish_messages: publisher.publish() × 2
       │   (6200 kp large + 2300 kp query)
       │
895 ms ─── [2s bekleme] create_timer(2.0, shutdown)
       │
2895 ms ─── Node kapanır, rclpy.shutdown()
```

### 5.2 Her Aşamanın Detayı

| # | Aşama Adı | İşlev Çağrısı | Açıklama | CPU/I/O |
|---|---|---|---|---|
| 1 | `1_image_loading` | `cv2.imread()` | İki görüntüyü diskten belleğe yükler | **I/O bound** (disk) |
| 2 | `2_grayscale_conversion` | `cv2.cvtColor(BGR→GRAY)` | Renkli → Gri tonlama dönüşümü. 3 kanal → 1 kanal | CPU bound |
| 2b | `2b_save_grayscale_images` | `cv2.imwrite()` | Ara görüntüleri diske kaydeder (debug) | I/O bound |
| 3 | `3_sift_large_image` | `sift.detectAndCompute(large)` | Büyük görüntüden SIFT öznitelikleri çıkarır. **En yavaş aşama.** | **CPU bound** |
| 4 | `4_sift_query_image` | `sift.detectAndCompute(query)` | Query görüntüsünden SIFT öznitelikleri çıkarır | CPU bound |
| 4b | `4b_save_keypoint_images` | `cv2.drawKeypoints()` + `cv2.imwrite()` | Anahtar nokta görselleştirmelerini kaydeder | CPU + I/O |
| 5 | `5_message_serialization` | `_build_feature_msg()` | OpenCV veri yapılarını ROS2 mesajlarına dönüştürür | CPU bound |
| 6 | `6_publish_messages` | `publisher.publish()` | 0.5s gecikmeden sonra mesajları topic'lere yayınlar | **Network/DDS** |

### 5.3 SIFT Algoritması Detayı

```python
sift = cv2.SIFT_create()  # Varsayılan parametrelerle
kp_large, des_large = sift.detectAndCompute(large_gray, None)
kp_query, des_query = sift.detectAndCompute(query_gray, None)
```

**SIFT (Scale-Invariant Feature Transform)** algoritması 4 ana aşamadan oluşur:

```
SIFT Algoritma Aşamaları:
┌──────────────────────────────────────────────────────────────────┐
│ 1. Ölçek-Uzay Oluşturma (Scale-Space Construction)              │
│                                                                  │
│   Görüntü → Gaussian filtreleme (σ = 1.6, 1.6×k, 1.6×k²...)    │
│           → Difference-of-Gaussian (DoG) hesaplama              │
│                                                                  │
│   Oktav 1: [960×640] → [480×320] → [240×160]                    │
│   Oktav 2: [480×320] → [240×160] → [120×80]                     │
│   Oktav 3: [240×160] → [120×80] → [60×40]                       │
│   Oktav 4: [120×80] → [60×40] → [30×20]                         │
└──────────────────────────────────────────────────────────────────┘
                              ▼
┌──────────────────────────────────────────────────────────────────┐
│ 2. Anahtar Nokta Tespiti (Keypoint Detection)                   │
│                                                                  │
│   • DoG uzayında yerel ekstremum noktaları bul (maks/min)       │
│   • 3×3×3 komşulukta karşılaştırma (26 komşu)                   │
│   • Alt-piksel konum belirleme (Taylor açılımı)                 │
│   • Düşük kontrastlı noktaları ele (contrastThreshold=0.04)     │
│   • Kenar noktalarını ele (edgeThreshold=10)                    │
└──────────────────────────────────────────────────────────────────┘
                              ▼
┌──────────────────────────────────────────────────────────────────┐
│ 3. Yön Atama (Orientation Assignment)                           │
│                                                                  │
│   • Her anahtar nokta çevresinde gradient hesapla                │
│   • 36-bin histogram (0°-360°, her 10°'de bir)                  │
│   • En yüksek bin → ana yön (angle)                             │
│   • Maksimumun %80'i üzerindeki diğer pikler → ek yönler        │
└──────────────────────────────────────────────────────────────────┘
                              ▼
┌──────────────────────────────────────────────────────────────────┐
│ 4. Tanımlayıcı Oluşturma (Descriptor Computation)               │
│                                                                  │
│   • 16×16 komşuluk bölgesi al (ana yöne göre döndürülmüş)       │
│   • 4×4 = 16 alt-bölgeye ayır                                    │
│   • Her alt-bölgede 8 yönlü gradient histogramı                  │
│   • 16 × 8 = 128 boyutlu vektör                                 │
│   • Normalizasyon + eşikleme (parlaklık değişimlerine dayanıklı) │
└──────────────────────────────────────────────────────────────────┘
```

**SIFT varsayılan parametreleri ve etkileri:**

| Parametre | Değer | Etkisi |
|---|---|---|
| `nfeatures` | 0 | Sınırsız — tüm öznitelikleri çıkar |
| `nOctaveLayers` | 3 | Her oktavdaki katman sayısı. Artırmak = daha çok anahtar nokta, daha yavaş |
| `contrastThreshold` | 0.04 | Düşük kontrastlı noktaları filtreler. Düşürmek = daha çok nokta |
| `edgeThreshold` | 10 | Kenar noktalarını filtreler. Düşürmek = daha az nokta |
| `sigma` | 1.6 | İlk Gaussian sigma değeri. Düşürmek = daha ince detaylar |

### 5.4 Yayınlama Stratejisi

```python
# Query mesajını önce yayınla (daha küçük payload, hızlı ulaşsın)
self._msg_query.publish_timestamp = time.time()
self.pub_query.publish(self._msg_query)

# Sonra large mesajını yayınla (daha büyük payload)
self._msg_large.publish_timestamp = time.time()
self.pub_large.publish(self._msg_large)
```

**Strateji açıklaması:** Query mesajı genellikle daha küçük olduğu için (daha az anahtar nokta) önce yayınlanır. Bu sayede subscriber her iki mesajı da yaklaşık aynı anda alır. Büyük mesajın DDS üzerinden taşınması daha uzun sürer.

**Zamanlama:**
- `create_timer(0.5, _publish_and_shutdown)` — 500ms gecikme, subscriber'ın bağlanması için
- `create_timer(2.0, lambda: rclpy.try_shutdown())` — 2000ms bekleme, mesajların teslimi için

### 5.5 Çıktı Görüntüleri (Publisher)

```
output/
├── publisher_1_large_grayscale.png    # Büyük görüntü gri tonlama
├── publisher_2_query_grayscale.png    # Query görüntüsü gri tonlama
├── publisher_3_large_keypoints.png    # Büyük görüntü anahtar noktaları
└── publisher_4_query_keypoints.png    # Query görüntüsü anahtar noktaları
```

---

## 6. Düğüm 2: flann_subscriber

### 6.1 Yaşam Döngüsü

```
Zaman Çizelgesi (ms olarak, örnek değerler):

 0 ms ─── Node.__init__() başlar
       │   2 subscription oluştur
       │   100ms periyodik timer başlat (_try_match)
       │
       │   [Bekleme — publisher SIFT çıkarıp yayınlayana kadar]
       │
700 ms ─── received_large_features (callback)
       │   6200 keypoints, ~3.2 MB payload
       │   transport_latency: ~2.3 ms
       │
700 ms ─── received_query_features (callback)
       │   2300 keypoints, ~1.2 MB payload
       │   transport_latency: ~0.8 ms
       │
       │   [Her iki mesaj da alındı → _try_match çalışır]
       │
701 ms ─── 1_data_reconstruction
       │   _to_keypoints() + _to_descriptors() × 2
       │   (15.2 ms)
       │
716 ms ─── 1b_save_received_keypoints_image
       │   drawKeypoints + imwrite × 2
       │   (10.5 ms)
       │
727 ms ─── 2_flann_index_build
       │   FlannBasedMatcher(index_params, search_params)
       │   (0.5 ms)
       │
728 ms ─── 3_flann_knn_matching
       │   flann.knnMatch(query_des, train_des, k=2)
       │   (125.4 ms, 2300 raw matches)
       │
853 ms ─── 4_lowes_ratio_test
       │   m.distance < 0.7 × n.distance
       │   (2.1 ms, 512 good matches, %77.7 rejection)
       │
855 ms ─── 4b_save_ratio_test_image
       │   drawMatches + imwrite
       │   (8.3 ms)
       │
863 ms ─── 5_point_extraction
       │   src_pts / dst_pts oluşturma
       │   (0.3 ms)
       │
864 ms ─── 6_homography_ransac
       │   findHomography(src, dst, RANSAC, 5.0)
       │   (1.8 ms, 420 inliers / 512 = %82.0)
       │
866 ms ─── 7_perspective_transform
       │   perspectiveTransform(corners, M)
       │   (0.1 ms)
       │
866 ms ─── 7b_save_ransac_stage_images
       │   inlier matches + detected region
       │   (12.4 ms)
       │
879 ms ─── 8_final_composite_figure
       │   matplotlib 4-panel figure
       │   (45.2 ms)
       │
924 ms ─── _finish(success=True)
       │   perf raporu yazdır + kaydet
       │   rclpy.shutdown()
```

### 6.2 Her Aşamanın Detayı

| # | Aşama Adı | Açıklama | Zaman (ms) |
|---|---|---|---|
| 1 | `1_data_reconstruction` | ROS2 mesajındaki float32[] dizilerini cv2.KeyPoint nesnelerine ve (N×128) numpy dizilerine dönüştürür | ~15 ms |
| 2 | `2_flann_index_build` | FLANN eşleştiriciyi oluşturur (KD-Tree, 5 ağaç, 50 kontrol) | ~0.5 ms |
| 3 | `3_flann_knn_matching` | **En ağır aşama.** Her query tanımlayıcısı için en yakın 2 komşuyu bulur | ~125 ms |
| 4 | `4_lowes_ratio_test` | Birinci mesafe ikincinin %70'inden küçükse → iyi eşleşme | ~2 ms |
| 5 | `5_point_extraction` | İyi eşleşmelerin koordinatlarını ayrı dizilere çıkarır | ~0.3 ms |
| 6 | `6_homography_ransac` | RANSAC ile homografi matrisini hesaplar (5.0 piksel eşik) | ~2 ms |
| 7 | `7_perspective_transform` | Query köşelerini homografi ile büyük görüntüye yansıtır | ~0.1 ms |
| 8 | `8_final_composite_figure` | 4 panelli matplotlib figürü oluşturup kaydeder | ~45 ms |

### 6.3 FLANN Eşleme Detayı

```
FLANN (Fast Library for Approximate Nearest Neighbors) — KDTREE Algoritması:

    Query Descriptors (2300 × 128)         Train Descriptors (6200 × 128)
    ┌─────────────────────────────┐        ┌─────────────────────────────┐
    │ d1: [0.12, 0.45, ...]      │        │ t1: [0.11, 0.44, ...]      │
    │ d2: [0.89, 0.23, ...]      │        │ t2: [0.90, 0.25, ...]      │
    │ d3: [0.34, 0.67, ...]      │        │ t3: [0.33, 0.68, ...]      │
    │ ...                        │        │ ...                         │
    │ d2300: [0.56, 0.78, ...]   │        │ t6200: [0.55, 0.79, ...]   │
    └──────────────┬──────────────┘        └──────────────┬──────────────┘
                   │                                      │
                   └──────────────┬───────────────────────┘
                                  │
                                  ▼
            ┌─────────────────────────────────────────┐
            │         KD-Tree Oluşturma               │
            │         (5 ağaç, rastgele bölümleme)    │
            │                                         │
            │    Ağaç 1:    Ağaç 2:    ...  Ağaç 5:   │
            │      ●          ●                ●      │
            │     / \        / \              / \     │
            │    ●   ●      ●   ●            ●   ●    │
            │   / \ / \    / \ / \          / \ / \   │
            │  ● ● ● ●   ● ● ● ●         ● ● ● ●    │
            └─────────────────────────────────────────┘
                                  │
                                  ▼
            ┌─────────────────────────────────────────┐
            │         kNN Arama (k=2)                 │
            │                                         │
            │  Her query için:                        │
            │  1. 5 ağaçta da ara → 5×50 = 250 aday  │
            │  2. Öklid mesafesi hesapla               │
            │  3. En yakın 2'yi seç                   │
            │                                         │
            │  Çıktı: matches[2300][2]                │
            │  Her satır: [en yakın, 2. en yakın]     │
            └─────────────────────────────────────────┘
                                  │
                                  ▼
            ┌─────────────────────────────────────────┐
            │         Lowe's Ratio Test               │
            │                                         │
            │  if m.distance < 0.7 × n.distance:      │
            │      → iyi eşleşme (benzersiz)          │
            │  else:                                  │
            │      → ret et (belirsiz/anlamlı değil)  │
            │                                         │
            │  Örnek: 2300 → 512 iyi (%22.3)         │
            │         %77.7 ret edildi                │
            └─────────────────────────────────────────┘
```

**FLANN parametreleri:**

```python
FLANN_INDEX_KDTREE = 1
index_params = dict(algorithm=FLANN_INDEX_KDTREE, trees=5)
search_params = dict(checks=50)
```

| Parametre | Değer | Açıklama | Etkisi |
|---|---|---|---|
| `algorithm` | 1 (KDTREE) | Kullanılacak indeksleme algoritması | KDTREE yüksek boyut için optimize |
| `trees` | 5 | Oluşturulacak KD-Ağacı sayısı | Daha çok ağaç = daha hızlı arama, daha yavaş indeks oluşturma |
| `checks` | 50 | Arama sırasında kontrol edilecek yaprak sayısı | Daha çok kontrol = daha doğru, daha yavaş |

### 6.4 RANSAC Homografi Detayı

```
RANSAC (Random Sample Consensus) — Homografi Hesaplama:

                              Giriş:
                src_pts (query)      dst_pts (large)
                ┌─────────┐          ┌─────────┐
                │ (x1,y1) │          │ (x1,y1) │
                │ (x2,y2) │          │ (x2,y2) │
                │ (x3,y3) │          │ (x3,y3) │
                │ ...     │          │ ...     │
                │ (xN,yN) │          │ (xN,yN) │
                └─────────┘          └─────────┘
                      │                    │
                      └──────┬─────────────┘
                             │
                             ▼
            ┌──────────────────────────────────────┐
            │  1. Rastgele 4 nokta seç             │
            │  2. 4 noktadan homografi matrisi(M)  │
            │     hesapla:                          │
            │     ┌             ┐                   │
            │     │ h11 h12 h13 │                   │
            │ M = │ h21 h22 h23 │                   │
            │     │ h31 h32 h33 │                   │
            │     └             ┘                   │
            │  3. Tüm noktaları M ile dönüştür      │
            │  4. Hata < 5.0 piksel → inlier        │
            │  5. Adım 1-4'ü N kere tekrarla        │
            │  6. En çok inlier veren M'i seç       │
            │  7. Tüm inlier'larla M'i yeniden hesapla│
            └──────────────────────────────────────┘
                             │
                             ▼
            ┌──────────────────────────────────────┐
            │  Çıktı:                              │
            │  - Homografi matrisi (3×3)            │
            │  - İç nokta (inlier) maske            │
            │  - Query köşelerinin büyük resimdeki  │
            │    konumu:                            │
            │    (x1,y1) ───────────────── (x2,y1) │
            │         │  TESPİT EDİLEN   │          │
            │         │     BÖLGE       │          │
            │    (x1,y2) ───────────────── (x2,y2) │
            └──────────────────────────────────────┘
```

### 6.5 Çıktı Görüntüleri (Subscriber)

```
output/
├── subscriber_1_large_received_keypoints.png    # Büyük → alınan anahtar noktalar
├── subscriber_2_query_received_keypoints.png    # Query → alınan anahtar noktalar
├── subscriber_3_ratio_test_matches.png          # Lowe's ratio test sonrası eşleşmeler
├── subscriber_4_ransac_inlier_matches.png       # RANSAC iç nokta eşleşmeleri
├── subscriber_5_detected_region.png             # Tespit edilen bölge (yeşil çerçeve)
└── subscriber_6_matching_result.png             # Final 4-panel kompozit figür
    (ve ayrıca: matching_result.png — geriye uyumluluk için proje kökünde)
```

### 6.6 Final Görselleştirme (4-Panel)

```
subscriber_6_matching_result.png

┌─────────────────────┬─────────────────────┐
│                     │                     │
│   QUERY IMAGE       │   TRAIN IMAGE       │
│   (test.jpg)        │   (ankara.jpg)      │
│                     │                     │
│                     │                     │
│                     │                     │
├─────────────────────┼─────────────────────┤
│                     │                     │
│   DETECTED REGION   │   SIFT MATCHES      │
│   (yeşil çerçeve)   │   (RANSAC inliers)  │
│                     │                     │
│                     │                     │
│                     │                     │
└─────────────────────┴─────────────────────┘
```

---

## 7. Performans Ölçüm Altyapısı: PerfTracker

`perf_utils.py` dosyası, her iki düğümde de kullanılan **performans ölçüm altyapısını** sağlar.

### 7.1 Sınıf Yapısı

```
PerfTracker
├── __init__(node_name, output_dir)
│   ├── start_wall   = datetime.now()      # Başlangıç saati (insan okuması)
│   ├── start_perf   = time.perf_counter()  # Yüksek çözünürlüklü zaman
│   ├── start_cpu    = time.process_time()  # CPU zamanı
│   ├── stages = OrderedDict()             # Aşama kayıtları
│   ├── events = OrderedDict()             # Zaman damgası olayları
│   └── extra  = OrderedDict()             # Ek bilgiler
│
├── stage(name) → context manager          # Aşama zamanlama
│   └── Ölçer: wall_time + cpu_time
│
├── mark_event(name) → float               # Anlık olay kaydı
├── add_info(key, value)                   # Ek bilgi ekle
├── add_stage_meta(name, key, value)       # Aşama metadata'sı ekle
│
├── build_report_lines() → [lines]         # Rapor metni oluştur
├── log_report(logger)                     # Terminale yazdır
└── save_report(basename) → (txt, json)   # Dosyaya kaydet
```

### 7.2 Zamanlama Mekanizması

```python
@contextmanager
def stage(self, name, **meta):
    """Context manager that times a stage and records wall + CPU time."""
    t0_wall = time.perf_counter()     # Yüksek çözünürlük (nanosaniye)
    t0_cpu = time.process_time()      # Sadece bu işlem için CPU süresi
    record = {'meta': dict(meta)}
    try:
        yield record                  # Aşama burada çalışır
    finally:
        wall_ms = (time.perf_counter() - t0_wall) * 1000.0
        cpu_ms = (time.process_time() - t0_cpu) * 1000.0
        record['wall_ms'] = wall_ms
        record['cpu_ms'] = cpu_ms
        record['started_at_ms'] = (t0_wall - self.start_perf) * 1000.0
        self.stages[name] = record
```

**Ölçülen metrikler:**
- **Wall time (duvar saati):** Gerçek dünya zamanı. I/O beklemeleri, context switch'ler dahil.
- **CPU time:** İşlemcinin sadece bu kod için harcadığı süre. Beklemeler hariç.
- **CPU/Wall oranı:** 1.0'a yakınsa → CPU-bound. Düşükse → I/O-bound.

### 7.3 Örnek Rapor Çıktısı

```
================================================================================
  PERFORMANCE REPORT  --  flann_subscriber
================================================================================
  Date/time        : 2024-06-10 14:23:45
  Host             : my-machine  (AMD64)
  Python           : 3.10.12
  PID              : 12345

  GENERAL INFO
  large_transport_latency_ms        : 2.345
  large_payload_mb                  : 3.17
  query_transport_latency_ms        : 0.812
  query_payload_mb                  : 1.18

  STAGE TIMINGS
  Stage                                   Wall (ms)    CPU (ms)   % wall
  1_data_reconstruction                     15.200      14.800    1.64%
      - large_keypoints: 6200
      - query_keypoints: 2300
  2_flann_index_build                        0.500       0.490    0.05%
      - algorithm: KD-Tree (trees=5, checks=50)
  3_flann_knn_matching                     125.400     124.800   13.55%
      - raw_matches: 2300
      - throughput: 18341 matches/s
  4_lowes_ratio_test                         2.100       2.080    0.23%
      - good_matches: 512
      - rejection_rate: 77.7%
  5_point_extraction                         0.300       0.290    0.03%
      - point_pairs: 512
  6_homography_ransac                        1.800       1.750    0.19%
      - inliers: 420/512 (82.0%)
  7_perspective_transform                    0.100       0.090    0.01%
  8_final_composite_figure                  45.200      44.100    4.88%
      - files: .../subscriber_6_matching_result.png
  --------------------------------------------------------------------------------
  Sum of measured stages                   190.600     188.400   20.58%
  TOTAL (since node start)                 925.800      45.200  100.00%

  TIMELINE EVENTS (ms since node start)
  received_large_features                 700.300 ms
  received_query_features                 700.300 ms
  matching_pipeline_started               700.500 ms
  matching_pipeline_finished              925.800 ms

  PROCESS RESOURCE USAGE
  Peak RSS memory                   : 245.30 MB
  User CPU time                     : 0.045 s
  System CPU time                   : 0.002 s
  Context switches (vol/invol)      : 125 / 3
  Page faults (minor/major)         : 452 / 0
  VMRSS                             : 245 MB
  VMHWM                             : 248 MB
================================================================================
```

### 7.4 JSON Rapor Yapısı

```json
{
  "node": "flann_subscriber",
  "timestamp": "2024-06-10T14:23:45.123456",
  "host": "my-machine",
  "python": "3.10.12",
  "pid": 12345,
  "total_wall_ms": 925.8,
  "total_cpu_ms": 45.2,
  "info": {
    "large_transport_latency_ms": "2.345",
    "large_payload_mb": "3.17",
    "query_transport_latency_ms": "0.812",
    "query_payload_mb": "1.18"
  },
  "stages": {
    "1_data_reconstruction": {
      "wall_ms": 15.2,
      "cpu_ms": 14.8,
      "started_at_ms": 700.5,
      "meta": {
        "large_keypoints": 6200,
        "query_keypoints": 2300
      }
    }
  },
  "events_ms": {
    "received_large_features": 700.3,
    "received_query_features": 700.3,
    "matching_pipeline_started": 700.5,
    "matching_pipeline_finished": 925.8
  },
  "resources": {
    "peak_rss_mb": 245.3,
    "user_cpu_s": 0.045,
    "system_cpu_s": 0.002
  }
}
```

---

## 8. Pipeline Akış Şeması (Sıralı)

Tüm pipeline'ın adım adım sıralı akışı:

```
ZAMAN (ms)   sift_publisher (PID X)                     flann_subscriber (PID Y)
══════════   ═══════════════════════                     ═══════════════════════════
    0        Node.init()
             │
    5        1_image_loading (imread)
             │  ┌──────────────────────────────────────┐
             │  │ ankarayı.jpg → 3 kanal BGR (960x640) │
             │  │ test.jpg      → 3 kanal BGR (480x320) │
             │  └──────────────────────────────────────┘
             │
   17        2_grayscale_conversion
             │  ┌──────────────────────────────────────┐
             │  │ BGR(3 kanal) → Gray(1 kanal)         │
             │  │ Her piksel: 0.299*R + 0.587*G + 0.114*B │
             │  └──────────────────────────────────────┘
             │
   22        2b_save_grayscale_images
             │  → publisher_1_large_grayscale.png
             │  → publisher_2_query_grayscale.png
             │
   27        3_sift_large_image
             │  detectAndCompute(large_gray)
             │  [===SIFT Algoritması===]
             │  → 6200 keypoints + 6200×128 descriptors
             │  throughput: ~12.4 Mpx/s
             │
  272        4_sift_query_image
             │  detectAndCompute(query_gray)
             │  → 2300 keypoints + 2300×128 descriptors
             │  throughput: ~11.8 Mpx/s
             │
  372        4b_save_keypoint_images
             │  → publisher_3_large_keypoints.png
             │  → publisher_4_query_keypoints.png
             │
  387        5_message_serialization
             │  2 × ImageFeatures mesajı oluştur
             │  large: ~3.2 MB payload (6200 kp)
             │  query: ~1.2 MB payload (2300 kp)
             │
  396        [500ms bekleme — subscriber bağlansın]
             │
  896        6_publish_messages
             │  ├── query mesajı → /sift/query_features
             │  └── large mesajı → /sift/large_features
             │                                          700  received_large_features
             │                                                transport_latency: ~2.3ms
             │                                          700  received_query_features
             │                                                transport_latency: ~0.8ms
             │
  898       [2000ms bekle — mesajlar teslim edilsin]    701  _try_match() → _done = True
                                                              │
             │                                           702  1_data_reconstruction
             │                                                (15.2ms)
             │                                           717  1b_save_received_keypoints_image
             │                                                (10.5ms)
             │                                           728  2_flann_index_build
             │                                                (0.5ms)
             │                                           729  3_flann_knn_matching
             │                                                [===FLANN===] (125.4ms)
             │                                           854  4_lowes_ratio_test (2.1ms)
             │                                           856  4b_save_ratio_test_image (8.3ms)
             │                                           864  5_point_extraction (0.3ms)
             │                                           865  6_homography_ransac (1.8ms)
             │                                           867  7_perspective_transform (0.1ms)
             │                                           867  7b_save_ransac_stage_images (12.4ms)
             │                                           880  8_final_composite_figure (45.2ms)
             │                                           925  _finish(success=True)
             │                                                └─ subscriber_performance.txt
             │                                                └─ pipeline_performance.json
             │                                                └─ rclpy.shutdown()
             │
 2898        rclpy.try_shutdown()
             └─ publisher_performance.txt
             └─ publisher_performance.json
```

---

## 9. Pipeline Çıktıları (Görsel)

### 9.1 Publisher Çıktıları

| # | Dosya | Açıklama |
|---|---|---|
| 1 | `publisher_1_large_grayscale.png` | Büyük görüntünün gri tonlama versiyonu |
| 2 | `publisher_2_query_grayscale.png` | Query görüntüsünün gri tonlama versiyonu |
| 3 | `publisher_3_large_keypoints.png` | Büyük görüntü üzerinde SIFT anahtar noktaları (daireler) |
| 4 | `publisher_4_query_keypoints.png` | Query görüntüsü üzerinde SIFT anahtar noktaları |

### 9.2 Subscriber Çıktıları

| # | Dosya | Açıklama |
|---|---|---|
| 1 | `subscriber_1_large_received_keypoints.png` | Alınan büyük görüntü + anahtar noktalar (yeşil) |
| 2 | `subscriber_2_query_received_keypoints.png` | Alınan query + anahtar noktalar (yeşil) |
| 3 | `subscriber_3_ratio_test_matches.png` | Lowe's ratio test sonrası eşleşen noktalar (çizgiler) |
| 4 | `subscriber_4_ransac_inlier_matches.png` | RANSAC doğrulamalı eşleşmeler (sadece inlier'lar) |
| 5 | `subscriber_5_detected_region.png` | Query'nin büyük resimde tespit edilen bölgesi (yeşil çerçeve) |
| 6 | `subscriber_6_matching_result.png` | 4 panelli final görselleştirme |

### 9.3 Performans Raporları

| Dosya | Format | Açıklama |
|---|---|---|
| `publisher_performance.txt` | Text | İnsan okunabilir publisher performans raporu |
| `publisher_performance.json` | JSON | Makine okunabilir publisher performans verisi |
| `subscriber_performance.txt` | Text | İnsan okunabilir subscriber performans raporu |
| `subscriber_performance.json` | JSON | Makine okunabilir subscriber performans verisi |
| `pipeline_performance.json` | JSON | **Birleşik pipeline raporu** — publisher + subscriber verilerini tek dosyada birleştirir |

### 9.4 Birleşik Pipeline Raporu

`pipeline_performance.json`, paralel sistemle karşılaştırma yapmak için oluşturulmuştur:

```json
{
  "node": "sequential_pipeline",
  "pid_publisher": 12344,
  "pid_subscriber": 12345,
  "publisher_total_wall_ms": 2898.5,
  "publisher_total_cpu_ms": 456.2,
  "subscriber_total_wall_ms": 925.8,
  "subscriber_total_cpu_ms": 45.2,
  "stages": {
    "pub_1_image_loading": { ... },
    "pub_2_grayscale_conversion": { ... },
    "pub_3_sift_large_image": { ... },
    "pub_4_sift_query_image": { ... },
    "pub_5_message_serialization": { ... },
    "pub_6_publish_messages": { ... },
    "sub_1_data_reconstruction": { ... },
    "sub_2_flann_index_build": { ... },
    "sub_3_flann_knn_matching": { ... },
    "sub_4_lowes_ratio_test": { ... },
    "sub_5_point_extraction": { ... },
    "sub_6_homography_ransac": { ... },
    "sub_7_perspective_transform": { ... },
    "sub_8_final_composite_figure": { ... }
  }
}
```

---

## 10. Performans Tablosu ve Yorumlama

### 10.1 Örnek Performans Verileri

```
┌────────────────────────────────────────────────────────────────────────┐
│                        PERFORMANCE TABLOSU                             │
├──────────────────────────────────────────────────────┬─────────────────┤
│ Aşama                                               │ Süre (ms)       │
├──────────────────────────────────────────────────────┼─────────────────┤
│ PUBLISHER                                           │                 │
│ 1_image_loading                                     │           12.3  │
│ 2_grayscale_conversion                              │            2.1  │
│ 3_sift_large_image (6200 kp)                        │          245.0  │
│ 4_sift_query_image (2300 kp)                        │          100.1  │
│ 5_message_serialization                             │            8.5  │
│ 6_publish_messages                                  │          450.2  │
│ Subtotal (publisher)                                 │          818.2  │
│                                                      │                 │
│ SUBSCRIBER                                           │                 │
│ Bekleme (node init → mesaj alımı)                    │          700.3  │
│ 1_data_reconstruction                                │           15.2  │
│ 2_flann_index_build                                  │            0.5  │
│ 3_flann_knn_matching (2300 raw)                     │          125.4  │
│ 4_lowes_ratio_test → 512 good                        │            2.1  │
│ 5_point_extraction                                  │            0.3  │
│ 6_homography_ransac (420 inliers)                    │            1.8  │
│ 7_perspective_transform                              │            0.1  │
│ 8_final_composite_figure                            │           45.2  │
│ Subtotal (subscriber)                                │          890.9  │
│                                                      │                 │
│ TOPLAM İŞLEM (publisher + subscriber)               │         ~1709.1 │
│ TOPLAM E2E (node start → bitiş)                     │         925.8   │
└──────────────────────────────────────────────────────┴─────────────────┘
```

### 10.2 Performans Yorumlama

#### Darboğaz Analizi

```
Zaman Dağılımı (subscriber, toplam 925.8 ms):
┌─────────────────────────────────────────────────────────────────┐
│ Bekleme (700.3 ms)  █████████████████████████████████████████    │ 75.6%
│                      (publisher SIFT çıkarırken bekleme)        │
├─────────────────────────────────────────────────────────────────┤
│ FLANN kNN (125.4 ms) ███████                                     │ 13.5%
├─────────────────────────────────────────────────────────────────┤
│ Final Figür (45.2ms) ██                                          │  4.9%
├─────────────────────────────────────────────────────────────────┤
│ Veri Yeniden Yapılandırma (15.2ms) █                             │  1.6%
├─────────────────────────────────────────────────────────────────┤
│ Diğer (39.7ms)       ██                                          │  4.3%
└─────────────────────────────────────────────────────────────────┘
```

**Önemli gözlemler:**

1. **En büyük zaman kaybı: Bekleme (%75.6)** — Subscriber, publisher'ın SIFT çıkarımını bitirmesini bekler. Bu **kaçınılmaz** bir sıralı bağımlılıktır.

2. **En ağır hesaplama: FLANN kNN eşleme (%13.5)** — 2300 query × 6200 train tanımlayıcısı arasında mesafe hesaplama. Karmaşıklık: O(N_query × N_train × 128).

3. **SIFT çıkarımı toplam: 345.1 ms** — Tüm pipeline'ın en yavaş aşaması (publisher tarafında). Büyük görüntü: 245 ms, query: 100 ms.

4. **Homografi çok hızlı: 1.8 ms** — 512 iyi eşleşme için RANSAC sadece 1.8 ms sürer.

5. **Görselleştirme önemli: 45.2 ms (%4.9)** — Matplotlib 4-panel figür oluşturma.

### 10.3 Performans Metrikleri

| Metrik | Değer | Açıklama |
|---|---|---|
| **SIFT çıktı hızı (large)** | ~12.4 Mpx/s | Büyük görüntü işleme hızı |
| **SIFT çıktı hızı (query)** | ~11.8 Mpx/s | Query görüntüsü işleme hızı |
| **FLANN eşleme hızı** | ~18341 matches/s | Saniyede eşleştirilen tanımlayıcı sayısı |
| **Lowe's ratio başarısı** | %22.3 | 2300 raw → 512 iyi (geçiş oranı) |
| **RANSAC başarısı** | %82.0 | 512 iyi → 420 inlier (doğrulama oranı) |
| **E2E gecikme** | 925.8 ms | Node başlangıcından bitişe kadar geçen süre |
| **CPU kullanımı** | 1 çekirdek | Sadece tek iş parçacığı kullanılır |
| **Tepe bellek (VMHWM)** | ~248 MB | En yüksek bellek kullanımı |

---

## 11. Sequential vs Paralel Karşılaştırması

### 11.1 Temel Farklar

| Özellik | Sequential | Paralel |
|---|---|---|
| **İş parçacığı** | 1 (tek CPU çekirdeği) | Çoklu (GPU + CPU) |
| **SIFT çıkarımı** | CPU'da sıralı (large → query) | GPU'da paralel (aynı anda) |
| **FLANN eşleme** | CPU'da sıralı | GPU'da paralel |
| **İletişim** | ROS2 topic (DDS) | Doğrudan bellek paylaşımı |
| **Ölçeklenebilirlik** | Düşük | Yüksek |
| **Gecikme** | Yüksek (bekleme + işlem) | Düşük (paralel) |
| **Karmaşıklık** | Düşük (basit, anlaşılır) | Yüksek (CUDA, senkronizasyon) |

### 11.2 Sıralı Pipeline'ın Avantajları

1. **Basitlik:** Kod okunabilirliği yüksek, hata ayıklaması kolay
2. **Taşınabilirlik:** Herhangi bir CPU'da çalışır (GPU gerektirmez)
3. **Deterministik:** Aynı girdi → aynı çıktı (paralel sistemde rastgelelik olabilir)
4. **Düşük geliştirme maliyeti:** CUDA bilgisi gerektirmez

### 11.3 Sıralı Pipeline'ın Dezavantajları

1. **Düşük performans:** CPU'da sıralı işlem → yavaş
2. **Kaynak israfı:** Çok çekirdekli CPU'ların potansiyeli kullanılmaz
3. **Yüksek gecikme:** SIFT çıkarımı ve eşleme sırayla yapılır
4. **Ölçeklenemez:** Daha büyük görüntülerde lineer olarak yavaşlar

### 11.4 Baseline Olarak Kullanımı

Bu sequential implementasyonu, paralel CUDA versiyonu için **referans noktası (baseline)** olarak kullanılır:

```
Hedef: Paralel sistem, sequential'den daha hızlı olmalı
Speedup = Sequential_süre / Paralel_süre

Örnek:
  Sequential SIFT (large): 245.0 ms
  Paralel SIFT (large):    45.2 ms
  Speedup: 245.0 / 45.2 ≈ 5.4× (5.4 kat hızlanma)

  Sequential FLANN: 125.4 ms
  Paralel FLANN:    18.7 ms
  Speedup: 125.4 / 18.7 ≈ 6.7× (6.7 kat hızlanma)
```

---

## Ek: Kod Referansları

| Dosya | Satır | İçerik |
|---|---|---|
| `sift_publisher.py` | 1-216 | Publisher düğümü |
| `sift_publisher.py` | 52-72 | Görüntü yükleme aşaması |
| `sift_publisher.py` | 74-79 | Gri tonlama dönüşümü |
| `sift_publisher.py` | 90-109 | SIFT çıkarımı (large + query) |
| `sift_publisher.py` | 126-155 | Mesaj serileştirme |
| `sift_publisher.py` | 157-175 | Mesaj oluşturma (`_build_feature_msg`) |
| `sift_publisher.py` | 177-206 | Yayınlama + kapanma |
| `flann_subscriber.py` | 1-422 | Subscriber düğümü |
| `flann_subscriber.py` | 87-107 | Veri yeniden yapılandırma (`_to_keypoints`, `_to_descriptors`) |
| `flann_subscriber.py` | 243-258 | FLANN indeks + eşleme |
| `flann_subscriber.py` | 260-273 | Lowe's ratio test |
| `flann_subscriber.py` | 292-299 | Nokta çıkarımı |
| `flann_subscriber.py` | 302-316 | Homografi (RANSAC) |
| `flann_subscriber.py` | 322-338 | Perspektif dönüşümü |
| `flann_subscriber.py` | 361-404 | Final görselleştirme |
| `flann_subscriber.py` | 114-125 | Performans raporu + kapanma |
| `flann_subscriber.py` | 127-194 | Birleşik pipeline raporu |
| `perf_utils.py` | 1-206 | Performans ölçüm altyapısı |
| `perf_utils.py` | 51-65 | Context manager ile aşama zamanlama |
| `perf_utils.py` | 67-70 | Anlık olay kaydı |
| `perf_utils.py` | 95-163 | Rapor oluşturma |
| `perf_utils.py` | 170-206 | Rapor kaydetme (txt + json) |

---

*Bu doküman, `src/sequential` paketinin sequential (sıralı) baseline implementasyonunu detaylı olarak açıklamak için hazırlanmıştır. Paralel CUDA versiyonu ile karşılaştırma yapmak için referans olarak kullanılabilir.*