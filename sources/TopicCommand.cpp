
#include "TopicCommand.hpp"

TopicCommand::TopicCommand(std::string command, std::shared_ptr<Client>& client, State& state) : ACommand(command, client, state) {}

std::unique_ptr<ACommand>	TopicCommand::create(std::string command, std::shared_ptr<Client>& client, State& state, \
	std::string channel, std::string topic) {
	TopicCommand*	cmd = new TopicCommand(command, client, state);

	if (channel.empty()) {
		cmd->_channel = channel;
		cmd->_error = true;
		cmd->_reply = ERR_NEEDMOREPARAMS;
	}
	else {
		auto chan = cmd->_state.getChannel(channel);
		cmd->_error = false;
		cmd->_set_topic = false;
		if (chan == cmd->_state.getChannels().end()) {
			cmd->_channel = "";
			cmd->_error = true;
			cmd->_reply = ERR_NOSUCHCHANNEL;
		}
		else if (!chan->isClient(cmd->_client)) {
			cmd->_channel = channel;
			cmd->_error = true;
			cmd->_reply = ERR_NOTONCHANNEL;
		}
		else {
			cmd->_channel = channel;
			if (!chan->isOperator(cmd->_client) && chan->getTopicMode()) {
				cmd->_error = true;
				cmd->_reply = ERR_CHANOPRIVSNEEDED;
			}
			else {
				cmd->_set_topic = true;
				cmd->_topic = topic;
			}
		}
	}
	return (std::unique_ptr<ACommand>(cmd));
}

void	TopicCommand::execute() const {
	Message msg;

	if (_error) {
		if (_channel.empty()) {
			msg.codedMessage(_client, _state, _reply, {});
		}
		else {
			msg.codedMessage(_client, _state, _reply, _channel);
		}
		return ;
	}
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
		msg.codedMessage(_client, _state, _reply, _channel);
		msg.codedMessage(_client, _state, _extra, _channel);
	}
}