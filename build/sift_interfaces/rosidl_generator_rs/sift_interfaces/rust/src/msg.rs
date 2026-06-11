#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};



// Corresponds to sift_interfaces__msg__ImageFeatures
/// Image meta information

#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ImageFeatures {

    // This member is not documented.
    #[allow(missing_docs)]
    pub image_name: std::string::String,


    // This member is not documented.
    #[allow(missing_docs)]
    pub height: i32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub width: i32,

    /// Keypoints (n keypoints)
    /// Each keypoint has: x, y, size, angle
    pub keypoints_x: Vec<f32>,


    // This member is not documented.
    #[allow(missing_docs)]
    pub keypoints_y: Vec<f32>,


    // This member is not documented.
    #[allow(missing_docs)]
    pub keypoints_size: Vec<f32>,


    // This member is not documented.
    #[allow(missing_docs)]
    pub keypoints_angle: Vec<f32>,

    /// Descriptors flattened: n x 128 floats
    /// Row-major order: descriptor_0[0..127], descriptor_1[0..127], ...
    pub descriptors: Vec<f32>,

    /// Number of keypoints
    pub num_keypoints: i32,

    /// Publish timestamp (seconds since epoch, set right before publishing)
    /// Used by the subscriber to measure transport latency
    pub publish_timestamp: f64,

}



impl Default for ImageFeatures {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::msg::rmw::ImageFeatures::default())
  }
}

impl rosidl_runtime_rs::Message for ImageFeatures {
  type RmwMsg = super::msg::rmw::ImageFeatures;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        image_name: msg.image_name.as_str().into(),
        height: msg.height,
        width: msg.width,
        keypoints_x: msg.keypoints_x.into(),
        keypoints_y: msg.keypoints_y.into(),
        keypoints_size: msg.keypoints_size.into(),
        keypoints_angle: msg.keypoints_angle.into(),
        descriptors: msg.descriptors.into(),
        num_keypoints: msg.num_keypoints,
        publish_timestamp: msg.publish_timestamp,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        image_name: msg.image_name.as_str().into(),
      height: msg.height,
      width: msg.width,
        keypoints_x: msg.keypoints_x.as_slice().into(),
        keypoints_y: msg.keypoints_y.as_slice().into(),
        keypoints_size: msg.keypoints_size.as_slice().into(),
        keypoints_angle: msg.keypoints_angle.as_slice().into(),
        descriptors: msg.descriptors.as_slice().into(),
      num_keypoints: msg.num_keypoints,
      publish_timestamp: msg.publish_timestamp,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      image_name: msg.image_name.to_string(),
      height: msg.height,
      width: msg.width,
      keypoints_x: msg.keypoints_x
          .into_iter()
          .collect(),
      keypoints_y: msg.keypoints_y
          .into_iter()
          .collect(),
      keypoints_size: msg.keypoints_size
          .into_iter()
          .collect(),
      keypoints_angle: msg.keypoints_angle
          .into_iter()
          .collect(),
      descriptors: msg.descriptors
          .into_iter()
          .collect(),
      num_keypoints: msg.num_keypoints,
      publish_timestamp: msg.publish_timestamp,
    }
  }
}


