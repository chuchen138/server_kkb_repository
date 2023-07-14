// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: message_info_base.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "message_info_base.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace sxg {

namespace {

const ::google::protobuf::Descriptor* MessageInfoBase_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  MessageInfoBase_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_message_5finfo_5fbase_2eproto() {
  protobuf_AddDesc_message_5finfo_5fbase_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "message_info_base.proto");
  GOOGLE_CHECK(file != NULL);
  MessageInfoBase_descriptor_ = file->message_type(0);
  static const int MessageInfoBase_offsets_[6] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MessageInfoBase, ver_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MessageInfoBase, publisher_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MessageInfoBase, sublisher_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MessageInfoBase, message_id_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MessageInfoBase, publish_time_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MessageInfoBase, content_),
  };
  MessageInfoBase_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      MessageInfoBase_descriptor_,
      MessageInfoBase::default_instance_,
      MessageInfoBase_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MessageInfoBase, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MessageInfoBase, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(MessageInfoBase));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_message_5finfo_5fbase_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    MessageInfoBase_descriptor_, &MessageInfoBase::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_message_5finfo_5fbase_2eproto() {
  delete MessageInfoBase::default_instance_;
  delete MessageInfoBase_reflection_;
}

void protobuf_AddDesc_message_5finfo_5fbase_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\027message_info_base.proto\022\003sxg\"\177\n\017Messag"
    "eInfoBase\022\013\n\003ver\030\001 \001(\005\022\021\n\tpublisher\030\002 \001("
    "\t\022\021\n\tsublisher\030\005 \001(\005\022\022\n\nmessage_id\030\003 \001(\005"
    "\022\024\n\014publish_time\030\004 \001(\005\022\017\n\007content\030\006 \001(\t", 159);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "message_info_base.proto", &protobuf_RegisterTypes);
  MessageInfoBase::default_instance_ = new MessageInfoBase();
  MessageInfoBase::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_message_5finfo_5fbase_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_message_5finfo_5fbase_2eproto {
  StaticDescriptorInitializer_message_5finfo_5fbase_2eproto() {
    protobuf_AddDesc_message_5finfo_5fbase_2eproto();
  }
} static_descriptor_initializer_message_5finfo_5fbase_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int MessageInfoBase::kVerFieldNumber;
const int MessageInfoBase::kPublisherFieldNumber;
const int MessageInfoBase::kSublisherFieldNumber;
const int MessageInfoBase::kMessageIdFieldNumber;
const int MessageInfoBase::kPublishTimeFieldNumber;
const int MessageInfoBase::kContentFieldNumber;
#endif  // !_MSC_VER

MessageInfoBase::MessageInfoBase()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void MessageInfoBase::InitAsDefaultInstance() {
}

MessageInfoBase::MessageInfoBase(const MessageInfoBase& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void MessageInfoBase::SharedCtor() {
  _cached_size_ = 0;
  ver_ = 0;
  publisher_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  sublisher_ = 0;
  message_id_ = 0;
  publish_time_ = 0;
  content_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

MessageInfoBase::~MessageInfoBase() {
  SharedDtor();
}

void MessageInfoBase::SharedDtor() {
  if (publisher_ != &::google::protobuf::internal::kEmptyString) {
    delete publisher_;
  }
  if (content_ != &::google::protobuf::internal::kEmptyString) {
    delete content_;
  }
  if (this != default_instance_) {
  }
}

void MessageInfoBase::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* MessageInfoBase::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return MessageInfoBase_descriptor_;
}

const MessageInfoBase& MessageInfoBase::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_message_5finfo_5fbase_2eproto();
  return *default_instance_;
}

MessageInfoBase* MessageInfoBase::default_instance_ = NULL;

MessageInfoBase* MessageInfoBase::New() const {
  return new MessageInfoBase;
}

void MessageInfoBase::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    ver_ = 0;
    if (has_publisher()) {
      if (publisher_ != &::google::protobuf::internal::kEmptyString) {
        publisher_->clear();
      }
    }
    sublisher_ = 0;
    message_id_ = 0;
    publish_time_ = 0;
    if (has_content()) {
      if (content_ != &::google::protobuf::internal::kEmptyString) {
        content_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool MessageInfoBase::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional int32 ver = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &ver_)));
          set_has_ver();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_publisher;
        break;
      }

      // optional string publisher = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_publisher:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_publisher()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->publisher().data(), this->publisher().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_message_id;
        break;
      }

      // optional int32 message_id = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_message_id:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &message_id_)));
          set_has_message_id();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(32)) goto parse_publish_time;
        break;
      }

      // optional int32 publish_time = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_publish_time:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &publish_time_)));
          set_has_publish_time();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(40)) goto parse_sublisher;
        break;
      }

      // optional int32 sublisher = 5;
      case 5: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_sublisher:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &sublisher_)));
          set_has_sublisher();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(50)) goto parse_content;
        break;
      }

      // optional string content = 6;
      case 6: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_content:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_content()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->content().data(), this->content().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void MessageInfoBase::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional int32 ver = 1;
  if (has_ver()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->ver(), output);
  }

  // optional string publisher = 2;
  if (has_publisher()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->publisher().data(), this->publisher().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      2, this->publisher(), output);
  }

  // optional int32 message_id = 3;
  if (has_message_id()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(3, this->message_id(), output);
  }

  // optional int32 publish_time = 4;
  if (has_publish_time()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(4, this->publish_time(), output);
  }

  // optional int32 sublisher = 5;
  if (has_sublisher()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(5, this->sublisher(), output);
  }

  // optional string content = 6;
  if (has_content()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->content().data(), this->content().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      6, this->content(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* MessageInfoBase::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional int32 ver = 1;
  if (has_ver()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->ver(), target);
  }

  // optional string publisher = 2;
  if (has_publisher()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->publisher().data(), this->publisher().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        2, this->publisher(), target);
  }

  // optional int32 message_id = 3;
  if (has_message_id()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(3, this->message_id(), target);
  }

  // optional int32 publish_time = 4;
  if (has_publish_time()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(4, this->publish_time(), target);
  }

  // optional int32 sublisher = 5;
  if (has_sublisher()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(5, this->sublisher(), target);
  }

  // optional string content = 6;
  if (has_content()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->content().data(), this->content().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        6, this->content(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int MessageInfoBase::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional int32 ver = 1;
    if (has_ver()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->ver());
    }

    // optional string publisher = 2;
    if (has_publisher()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->publisher());
    }

    // optional int32 sublisher = 5;
    if (has_sublisher()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->sublisher());
    }

    // optional int32 message_id = 3;
    if (has_message_id()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->message_id());
    }

    // optional int32 publish_time = 4;
    if (has_publish_time()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->publish_time());
    }

    // optional string content = 6;
    if (has_content()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->content());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void MessageInfoBase::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const MessageInfoBase* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const MessageInfoBase*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void MessageInfoBase::MergeFrom(const MessageInfoBase& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_ver()) {
      set_ver(from.ver());
    }
    if (from.has_publisher()) {
      set_publisher(from.publisher());
    }
    if (from.has_sublisher()) {
      set_sublisher(from.sublisher());
    }
    if (from.has_message_id()) {
      set_message_id(from.message_id());
    }
    if (from.has_publish_time()) {
      set_publish_time(from.publish_time());
    }
    if (from.has_content()) {
      set_content(from.content());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void MessageInfoBase::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void MessageInfoBase::CopyFrom(const MessageInfoBase& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool MessageInfoBase::IsInitialized() const {

  return true;
}

void MessageInfoBase::Swap(MessageInfoBase* other) {
  if (other != this) {
    std::swap(ver_, other->ver_);
    std::swap(publisher_, other->publisher_);
    std::swap(sublisher_, other->sublisher_);
    std::swap(message_id_, other->message_id_);
    std::swap(publish_time_, other->publish_time_);
    std::swap(content_, other->content_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata MessageInfoBase::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = MessageInfoBase_descriptor_;
  metadata.reflection = MessageInfoBase_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace sxg

// @@protoc_insertion_point(global_scope)
