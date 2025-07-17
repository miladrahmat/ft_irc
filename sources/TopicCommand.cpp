
#include "TopicCommand.hpp"

TopicCommand::TopicCommand(std::string command, std::shared_ptr<Client>& client, State& state) :
	ACommand(command, client, state) {}

std::unique_ptr<ACommand> TopicCommand::create(std::string command, std::shared_ptr<Client>& client,
	State& state, std::string channel, std::string topic) {

	TopicCommand* cmd = new TopicCommand(command, client, state);
	Message msg;

	cmd->_channel = channel;
	if (cmd->_channel.empty()) {
		msg.codedMessage(cmd->_client, cmd->_state, ERR_NEEDMOREPARAMS, {});
		delete cmd;
		return (nullptr);
	}
	auto chan = cmd->_state.getChannel(cmd->_channel);
	cmd->_set_topic = false;
	if (chan == cmd->_state.getChannels().end()) {
		msg.codedMessage(cmd->_client, cmd->_state, ERR_NOSUCHCHANNEL, cmd->_channel);
		delete cmd;
		return (nullptr);
	}
	if (!chan->isClient(cmd->_client)) {
		msg.codedMessage(cmd->_client, cmd->_state, ERR_NOTONCHANNEL, cmd->_channel);
		delete cmd;
		return (nullptr);
	}
	if (!chan->isOperator(cmd->_client) && chan->getTopicMode()) {
		msg.codedMessage(cmd->_client, cmd->_state, ERR_CHANOPRIVSNEEDED, cmd->_channel);
		delete cmd;
		return (nullptr);
	}
	cmd->_set_topic = true;
	cmd->_topic = topic;
	return (std::unique_ptr<ACommand>(cmd));
}

void TopicCommand::execute() const {

	Message msg;

	if (_set_topic) {
		auto chan = _state.getChannel(_channel);
		if (_topic.empty()) {
			chan->topic_who.reset();
			chan->topic_when = 0;
		}
		chan->setTopic(_client, _topic);
		chan->sendMsgToAll(_client, _command, _channel, _topic);
		msg.message(_client, _client, _command, _channel, _topic);
	}
	else {
		if (_state.getChannel(_channel)->topic.empty()) {
			msg.codedMessage(_client, _state, RPL_NOTOPIC, _channel);
		}
		else {
			auto it = _state.getChannel(_channel);
			struct reply rply = RPL_TOPIC;
			struct reply extra = RPL_TOPICWHOTIME;
			rply.msg = it->topic;
			extra.msg = it->topic_who->getNickname() + "!" + it->topic_who->getUsername() + "@" + it->topic_who->getHostname() + " " + std::to_string(it->topic_when);
			msg.codedMessage(_client, _state, rply, _channel);
			msg.codedMessage(_client, _state, extra, _channel);
		}
	}
}