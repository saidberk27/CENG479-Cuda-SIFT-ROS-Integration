# generated from rosidl_generator_py/resource/_idl.py.em
# with input from sift_interfaces:msg/ImageFeatures.idl
# generated code does not contain a copyright notice

# This is being done at the module level and not on the instance level to avoid looking
# for the same variable multiple times on each instance. This variable is not supposed to
# change during runtime so it makes sense to only look for it once.
from os import getenv

ros_python_check_fields = getenv('ROS_PYTHON_CHECK_FIELDS', default='')


# Import statements for member types

# Member 'keypoints_x'
# Member 'keypoints_y'
# Member 'keypoints_size'
# Member 'keypoints_angle'
# Member 'descriptors'
import array  # noqa: E402, I100

import builtins  # noqa: E402, I100

import math  # noqa: E402, I100

import rosidl_parser.definition  # noqa: E402, I100


class Metaclass_ImageFeatures(type):
    """Metaclass of message 'ImageFeatures'."""

    _CREATE_ROS_MESSAGE = None
    _CONVERT_FROM_PY = None
    _CONVERT_TO_PY = None
    _DESTROY_ROS_MESSAGE = None
    _TYPE_SUPPORT = None

    __constants = {
    }

    @classmethod
    def __import_type_support__(cls):
        try:
            from rosidl_generator_py import import_type_support
            module = import_type_support('sift_interfaces')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'sift_interfaces.msg.ImageFeatures')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__msg__image_features
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__msg__image_features
            cls._CONVERT_TO_PY = module.convert_to_py_msg__msg__image_features
            cls._TYPE_SUPPORT = module.type_support_msg__msg__image_features
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__msg__image_features

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class ImageFeatures(metaclass=Metaclass_ImageFeatures):
    """Message class 'ImageFeatures'."""

    __slots__ = [
        '_image_name',
        '_height',
        '_width',
        '_keypoints_x',
        '_keypoints_y',
        '_keypoints_size',
        '_keypoints_angle',
        '_descriptors',
        '_num_keypoints',
        '_publish_timestamp',
        '_check_fields',
    ]

    _fields_and_field_types = {
        'image_name': 'string',
        'height': 'int32',
        'width': 'int32',
        'keypoints_x': 'sequence<float>',
        'keypoints_y': 'sequence<float>',
        'keypoints_size': 'sequence<float>',
        'keypoints_angle': 'sequence<float>',
        'descriptors': 'sequence<float>',
        'num_keypoints': 'int32',
        'publish_timestamp': 'double',
    }

    # This attribute is used to store an rosidl_parser.definition variable
    # related to the data type of each of the components the message.
    SLOT_TYPES = (
        rosidl_parser.definition.UnboundedString(),  # noqa: E501
        rosidl_parser.definition.BasicType('int32'),  # noqa: E501
        rosidl_parser.definition.BasicType('int32'),  # noqa: E501
        rosidl_parser.definition.UnboundedSequence(rosidl_parser.definition.BasicType('float')),  # noqa: E501
        rosidl_parser.definition.UnboundedSequence(rosidl_parser.definition.BasicType('float')),  # noqa: E501
        rosidl_parser.definition.UnboundedSequence(rosidl_parser.definition.BasicType('float')),  # noqa: E501
        rosidl_parser.definition.UnboundedSequence(rosidl_parser.definition.BasicType('float')),  # noqa: E501
        rosidl_parser.definition.UnboundedSequence(rosidl_parser.definition.BasicType('float')),  # noqa: E501
        rosidl_parser.definition.BasicType('int32'),  # noqa: E501
        rosidl_parser.definition.BasicType('double'),  # noqa: E501
    )

    def __init__(self, **kwargs):
        if 'check_fields' in kwargs:
            self._check_fields = kwargs['check_fields']
        else:
            self._check_fields = ros_python_check_fields == '1'
        if self._check_fields:
            assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
                'Invalid arguments passed to constructor: %s' % \
                ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        self.image_name = kwargs.get('image_name', str())
        self.height = kwargs.get('height', int())
        self.width = kwargs.get('width', int())
        self.keypoints_x = array.array('f', kwargs.get('keypoints_x', []))
        self.keypoints_y = array.array('f', kwargs.get('keypoints_y', []))
        self.keypoints_size = array.array('f', kwargs.get('keypoints_size', []))
        self.keypoints_angle = array.array('f', kwargs.get('keypoints_angle', []))
        self.descriptors = array.array('f', kwargs.get('descriptors', []))
        self.num_keypoints = kwargs.get('num_keypoints', int())
        self.publish_timestamp = kwargs.get('publish_timestamp', float())

    def __repr__(self):
        typename = self.__class__.__module__.split('.')
        typename.pop()
        typename.append(self.__class__.__name__)
        args = []
        for s, t in zip(self.get_fields_and_field_types().keys(), self.SLOT_TYPES):
            field = getattr(self, s)
            fieldstr = repr(field)
            # We use Python array type for fields that can be directly stored
            # in them, and "normal" sequences for everything else.  If it is
            # a type that we store in an array, strip off the 'array' portion.
            if (
                isinstance(t, rosidl_parser.definition.AbstractSequence) and
                isinstance(t.value_type, rosidl_parser.definition.BasicType) and
                t.value_type.typename in ['float', 'double', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'int64', 'uint64']
            ):
                if len(field) == 0:
                    fieldstr = '[]'
                else:
                    if self._check_fields:
                        assert fieldstr.startswith('array(')
                    prefix = "array('X', "
                    suffix = ')'
                    fieldstr = fieldstr[len(prefix):-len(suffix)]
            args.append(s + '=' + fieldstr)
        return '%s(%s)' % ('.'.join(typename), ', '.join(args))

    def __eq__(self, other):
        if not isinstance(other, self.__class__):
            return False
        if self.image_name != other.image_name:
            return False
        if self.height != other.height:
            return False
        if self.width != other.width:
            return False
        if self.keypoints_x != other.keypoints_x:
            return False
        if self.keypoints_y != other.keypoints_y:
            return False
        if self.keypoints_size != other.keypoints_size:
            return False
        if self.keypoints_angle != other.keypoints_angle:
            return False
        if self.descriptors != other.descriptors:
            return False
        if self.num_keypoints != other.num_keypoints:
            return False
        if self.publish_timestamp != other.publish_timestamp:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @builtins.property
    def image_name(self):
        """Message field 'image_name'."""
        return self._image_name

    @image_name.setter
    def image_name(self, value):
        if self._check_fields:
            assert \
                isinstance(value, str), \
                "The 'image_name' field must be of type 'str'"
        self._image_name = value

    @builtins.property
    def height(self):
        """Message field 'height'."""
        return self._height

    @height.setter
    def height(self, value):
        if self._check_fields:
            assert \
                isinstance(value, int), \
                "The 'height' field must be of type 'int'"
            assert value >= -2147483648 and value < 2147483648, \
                "The 'height' field must be an integer in [-2147483648, 2147483647]"
        self._height = value

    @builtins.property
    def width(self):
        """Message field 'width'."""
        return self._width

    @width.setter
    def width(self, value):
        if self._check_fields:
            assert \
                isinstance(value, int), \
                "The 'width' field must be of type 'int'"
            assert value >= -2147483648 and value < 2147483648, \
                "The 'width' field must be an integer in [-2147483648, 2147483647]"
        self._width = value

    @builtins.property
    def keypoints_x(self):
        """Message field 'keypoints_x'."""
        return self._keypoints_x

    @keypoints_x.setter
    def keypoints_x(self, value):
        if self._check_fields:
            if isinstance(value, array.array):
                assert value.typecode == 'f', \
                    "The 'keypoints_x' array.array() must have the type code of 'f'"
                self._keypoints_x = value
                return
            from collections.abc import Sequence
            from collections.abc import Set
            from collections import UserList
            from collections import UserString
            assert \
                ((isinstance(value, Sequence) or
                  isinstance(value, Set) or
                  isinstance(value, UserList)) and
                 not isinstance(value, str) and
                 not isinstance(value, UserString) and
                 all(isinstance(v, float) for v in value) and
                 all(not (val < -3.402823466e+38 or val > 3.402823466e+38) or math.isinf(val) for val in value)), \
                "The 'keypoints_x' field must be a set or sequence and each value of type 'float' and each float in [-340282346600000016151267322115014000640.000000, 340282346600000016151267322115014000640.000000]"
        self._keypoints_x = array.array('f', value)

    @builtins.property
    def keypoints_y(self):
        """Message field 'keypoints_y'."""
        return self._keypoints_y

    @keypoints_y.setter
    def keypoints_y(self, value):
        if self._check_fields:
            if isinstance(value, array.array):
                assert value.typecode == 'f', \
                    "The 'keypoints_y' array.array() must have the type code of 'f'"
                self._keypoints_y = value
                return
            from collections.abc import Sequence
            from collections.abc import Set
            from collections import UserList
            from collections import UserString
            assert \
                ((isinstance(value, Sequence) or
                  isinstance(value, Set) or
                  isinstance(value, UserList)) and
                 not isinstance(value, str) and
                 not isinstance(value, UserString) and
                 all(isinstance(v, float) for v in value) and
                 all(not (val < -3.402823466e+38 or val > 3.402823466e+38) or math.isinf(val) for val in value)), \
                "The 'keypoints_y' field must be a set or sequence and each value of type 'float' and each float in [-340282346600000016151267322115014000640.000000, 340282346600000016151267322115014000640.000000]"
        self._keypoints_y = array.array('f', value)

    @builtins.property
    def keypoints_size(self):
        """Message field 'keypoints_size'."""
        return self._keypoints_size

    @keypoints_size.setter
    def keypoints_size(self, value):
        if self._check_fields:
            if isinstance(value, array.array):
                assert value.typecode == 'f', \
                    "The 'keypoints_size' array.array() must have the type code of 'f'"
                self._keypoints_size = value
                return
            from collections.abc import Sequence
            from collections.abc import Set
            from collections import UserList
            from collections import UserString
            assert \
                ((isinstance(value, Sequence) or
                  isinstance(value, Set) or
                  isinstance(value, UserList)) and
                 not isinstance(value, str) and
                 not isinstance(value, UserString) and
                 all(isinstance(v, float) for v in value) and
                 all(not (val < -3.402823466e+38 or val > 3.402823466e+38) or math.isinf(val) for val in value)), \
                "The 'keypoints_size' field must be a set or sequence and each value of type 'float' and each float in [-340282346600000016151267322115014000640.000000, 340282346600000016151267322115014000640.000000]"
        self._keypoints_size = array.array('f', value)

    @builtins.property
    def keypoints_angle(self):
        """Message field 'keypoints_angle'."""
        return self._keypoints_angle

    @keypoints_angle.setter
    def keypoints_angle(self, value):
        if self._check_fields:
            if isinstance(value, array.array):
                assert value.typecode == 'f', \
                    "The 'keypoints_angle' array.array() must have the type code of 'f'"
                self._keypoints_angle = value
                return
            from collections.abc import Sequence
            from collections.abc import Set
            from collections import UserList
            from collections import UserString
            assert \
                ((isinstance(value, Sequence) or
                  isinstance(value, Set) or
                  isinstance(value, UserList)) and
                 not isinstance(value, str) and
                 not isinstance(value, UserString) and
                 all(isinstance(v, float) for v in value) and
                 all(not (val < -3.402823466e+38 or val > 3.402823466e+38) or math.isinf(val) for val in value)), \
                "The 'keypoints_angle' field must be a set or sequence and each value of type 'float' and each float in [-340282346600000016151267322115014000640.000000, 340282346600000016151267322115014000640.000000]"
        self._keypoints_angle = array.array('f', value)

    @builtins.property
    def descriptors(self):
        """Message field 'descriptors'."""
        return self._descriptors

    @descriptors.setter
    def descriptors(self, value):
        if self._check_fields:
            if isinstance(value, array.array):
                assert value.typecode == 'f', \
                    "The 'descriptors' array.array() must have the type code of 'f'"
                self._descriptors = value
                return
            from collections.abc import Sequence
            from collections.abc import Set
            from collections import UserList
            from collections import UserString
            assert \
                ((isinstance(value, Sequence) or
                  isinstance(value, Set) or
                  isinstance(value, UserList)) and
                 not isinstance(value, str) and
                 not isinstance(value, UserString) and
                 all(isinstance(v, float) for v in value) and
                 all(not (val < -3.402823466e+38 or val > 3.402823466e+38) or math.isinf(val) for val in value)), \
                "The 'descriptors' field must be a set or sequence and each value of type 'float' and each float in [-340282346600000016151267322115014000640.000000, 340282346600000016151267322115014000640.000000]"
        self._descriptors = array.array('f', value)

    @builtins.property
    def num_keypoints(self):
        """Message field 'num_keypoints'."""
        return self._num_keypoints

    @num_keypoints.setter
    def num_keypoints(self, value):
        if self._check_fields:
            assert \
                isinstance(value, int), \
                "The 'num_keypoints' field must be of type 'int'"
            assert value >= -2147483648 and value < 2147483648, \
                "The 'num_keypoints' field must be an integer in [-2147483648, 2147483647]"
        self._num_keypoints = value

    @builtins.property
    def publish_timestamp(self):
        """Message field 'publish_timestamp'."""
        return self._publish_timestamp

    @publish_timestamp.setter
    def publish_timestamp(self, value):
        if self._check_fields:
            assert \
                isinstance(value, float), \
                "The 'publish_timestamp' field must be of type 'float'"
            assert not (value < -1.7976931348623157e+308 or value > 1.7976931348623157e+308) or math.isinf(value), \
                "The 'publish_timestamp' field must be a double in [-1.7976931348623157e+308, 1.7976931348623157e+308]"
        self._publish_timestamp = value
