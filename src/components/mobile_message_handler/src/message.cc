#include "mobile_message_handler/message.h"
#include "Utils/macro.h"

namespace mobile_message_handler {

Message::Message()
  : function_id_(0)
  , type_(Unknown)
  , correlation_id_(0)
  , connection_key_(0)
  , binary_data_(NULL) {

}

Message::Message(const Message& message) {
	*this = message;
}

Message& Message::operator=(const Message& message) {
	set_function_id(message.function_id_);
	set_correlation_id(message.correlation_id_);
	set_connection_key(message.connection_key_);
	set_message_type(message.type_);
	set_binary_data(message.binary_data_);
	set_json_message(message.json_message_);

	return *this;
}

Message::~Message() {
}

int Message::function_id() const {
	return function_id_;
}

int Message::correlation_id() const {
	return correlation_id_;
}

int Message::connection_key() const {
	return connection_key_;
}

MessageType Message::type() const {
	return type_;
}

const std::string& Message::json_message() const {
	return json_message_;
}

const BinaryData* Message::binary_data() const {
	return binary_data_;
}

bool Message::hasBinaryData() const {
	return (binary_data_ != NULL);
}

void Message::set_function_id(int id) {
	function_id_ = id;
}

void Message::set_correlation_id(int id) {
	correlation_id_ = id;
}

void Message::set_connection_key(int key) {
	connection_key_ = key;
}

void Message::set_message_type(MessageType type) {
	type_ = type;
}

void Message::set_binary_data(BinaryData* data) {
	if (NULL == data) {
		// Please, don't add NOTREACHED() here.
		// We can copy object without binary data using copy ctor and operator=
		return;
	}

	binary_data_ = data;
}

void Message::set_json_message(const std::string& json_message) {
	json_message_ = json_message;
}

}  // namespace mobile_message_handler
