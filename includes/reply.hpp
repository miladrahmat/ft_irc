
#pragma once

#include <string>
#define CHAN_MAX 50

struct reply {
    std::string code;
    std::string msg;
};

static const reply SUCCESS = {"000", ""};
static const reply ERROR = {"ERROR", ""};
static const reply RPL_WELCOME = {"001", "\e[1;96mWelcome to GALLERIA!\e[0m"};
static const reply RPL_YOURHOST = {"002", "\e[0;96mYour host is Galleria\e[0m"}; 
static const reply RPL_CREATED = {"003", "This server was created today"};
static const reply RPL_MYINFO = {"004", "ircserv.galleria 1.0   iklot"};
static const reply RPL_ISUPPORT = {"005", "CHANTYPES=# NICKLEN=15 PREFIX=(o)@ CHANMODES=klit :are supported by this server"};
static const reply RPL_ENDOFWHO = {"315", "End of /WHO list"};
static const reply RPL_ENDOFWHOIS = {"318", "End of /WHOIS list"};
static reply RPL_CHANNELMODEIS = {"324", ""};
static const reply RPL_NOTOPIC = {"331", "No topic is set"};
static reply RPL_TOPIC = {"332", ""};
static reply RPL_TOPICWHOTIME  = {"333", ""};
static const reply RPL_INVITING = {"341", ""};
static reply RPL_WHOREPLY = {"352", "0 "};
static const reply RPL_NAMREPLY = {"353", ""};
static const reply RPL_ENDOFNAMES = {"366", "End of /NAMES list"};
static reply ERR_UNKNOWNERROR = {"400", ""};
static const reply ERR_NOSUCHNICK = {"401", "No such nick/channel"};
static const reply ERR_NOSUCHSERVER = {"402", "No such server"};
static const reply ERR_CANNOTSENDTOCHAN = {"404", "Cannot send to channel"};
static const reply ERR_NOSUCHCHANNEL = {"403",  "No such channel"};
static const reply ERR_TOOMANYCHANNELS = {"405", "You have joined too many channels"};
static const reply ERR_UNKNOWNCOMMAND = {"421", "Unknown command"};
static const reply ERR_NONICKNAMEGIVEN = {"431", "No nickname given"};
static const reply ERR_ERRONEUSNICKNAME = {"432", "Erroneus nickname"};
static const reply ERR_NICKNAMEINUSE = {"433", "Nickname is already in use."};
static const reply ERR_USERNOTINCHANNEL = {"441", "They aren't on that channel"};
static const reply ERR_NOTONCHANNEL = {"442", "You're not on that channel"};
static const reply ERR_USERONCHANNEL = {"443", "is already on channel"};
static const reply ERR_NOTREGISTERED = {"451", "You have not registered"};
static const reply ERR_NEEDMOREPARAMS = {"461", "Not enough parameters"};
static const reply ERR_ALREADYREGISTERED = {"462", "You may not reregister"};
static const reply ERR_PASSWDMISMATCH = {"464", "Password incorrect"};
static const reply ERR_CHANNELISFULL = {"471", "Cannot join channel (+l)"};
static const reply ERR_UNKNOWNMODE = {"472", "is unknown mode char to me"};
static const reply ERR_INVITEONLYCHAN = {"473", "Cannot join channel (+i)"};
static const reply ERR_BADCHANNELKEY = {"475", "Cannot join channel (+k)"};
static const reply ERR_BADCHANMASK = {"476", "Bad Channel Mask"};
static const reply ERR_BADCHANNAME = {"479", "Illegal channel name"};
static const reply ERR_CHANOPRIVSNEEDED = {"482", "You're not channel operator"};
static const reply ERR_UMODEUNKNOWNFLAG = {"501", "Unknown MODE flag"};
static const reply ERR_USERSDONTMATCH = {"502", "Cant change mode for other users"};