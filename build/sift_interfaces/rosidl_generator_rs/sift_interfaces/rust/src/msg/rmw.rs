#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};


#[link(name = "sift_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__sift_interfaces__msg__ImageFeatures() -> *const std::ffi::c_void;
}

#[link(name = "sift_interfaces__rosidl_generator_c")]
extern "C" {
    fn sift_interfaces__msg__ImageFeatures__init(msg: *mut ImageFeatures) -> bool;
    fn sift_interfaces__msg__ImageFeatures__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<ImageFeatures>, size: usize) -> bool;
    fn sift_interfaces__msg__ImageFeatures__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<ImageFeatures>);
    fn sift_interfaces__msg__ImageFeatures__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<ImageFeatures>, out_seq: *mut rosidl_runtime_rs::Sequence<ImageFeatures>) -> bool;
}

// Corresponds to sift_interfaces__msg__ImageFeatures
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]

/// Image meta information

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ImageFeatures {

    // This member is not documented.
    #[allow(missing_docs)]
    pub image_name: rosidl_runtime_rs::String,


    // This member is not documented.
    #[allow(missing_docs)]
    pub height: i32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub width: i32,

    /// Keypoints (n keypoints)
    /// Each keypoint has: x, y, size, angle
    pub keypoints_x: rosidl_runtime_rs::Sequence<f32>,


    // This member is not documented.
    #[allow(missing_docs)]
    pub keypoints_y: rosidl_runtime_rs::Sequence<f32>,


    // This member is not documented.
    #[allow(missing_docs)]
    pub keypoints_size: rosidl_runtime_rs::Sequence<f32>,


    // This member is not documented.
    #[allow(missing_docs)]
    pub keypoints_angle: rosidl_runtime_rs::Sequence<f32>,

    /// Descriptors flattened: n x 128 floats
    /// Row-major order: descriptor_0[0..127], descriptor_1[0..127], ...
    pub descriptors: rosidl_runtime_rs::Sequence<f32>,

    /// Number of keypoints
    pub num_keypoints: i32,

    /// Publish timestamp (seconds since epoch, set right before publishing)
    /// Used by the subscriber to measure transport latency
    pub publish_timestamp: f64,

}



impl Default for ImageFeatures {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !sift_interfaces__msg__ImageFeatures__init(&mut msg as *mut _) {
        panic!("Call to sift_interfaces__msg__ImageFeatures__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for ImageFeatures {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { sift_interfaces__msg__ImageFeatures__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { sift_interfaces__msg__ImageFeatures__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { sift_interfaces__msg__ImageFeatures__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for ImageFeatures {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for ImageFeatures where Self: Sized {
  const TYPE_NAME: &'static str = "sift_interfaces/msg/ImageFeatures";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__sift_interfaces__msg__ImageFeatures() }
  }
}


