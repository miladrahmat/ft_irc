#pragma once
#include <string>

struct reply {
    std::string code;
    std::string msg;
};

static const reply SUCCESS = {"000", ""};
static const reply RPL_WELCOME = {"001", "Welcome to IRC"};
static const reply RPL_YOURHOST = {"002", "Your host is ircserv"}; 
static const reply RPL_CREATED = {"003", "This server was created today"};
static const reply RPL_MYINFO = {"004", " ircserv 1.0 o o"};
static const reply RPL_ISUPPORT = {"005", " CHANTYPES=#& NICKLEN=15 PREFIX=(ov)@+ CHANMODES=itkol :are supported by this server"};
static reply RPL_TOPIC = {"332", " :"};
//static const err RPL_TOPICWHOTIME  = {"333"};
static const reply RPL_NAMREPLY = {"353", ""};
static const reply RPL_ENDOFNAMES = {"366", "End of /NAMES list"};
static const reply ERR_NOSUCHNICK = {"401", "No such nick/channel"};
static const reply ERR_CANNOTSENDTOCHAN = {"404", "Cannot send to channel"};
static const reply ERR_NOSUCHCHANNEL = {"403",  "No such channel"};
static const reply ERR_TOOMANYCHANNELS = {"405", "You have joined too many channels"};
static const reply ERR_NONICKNAMEGIVEN = {"431", "No nickname given"};
static const reply ERR_ERRONEUSNICKNAME = {"432", "Erroneus nickname"};
static const reply ERR_NICKNAMEINUSE = {"433", "Nickname is already in use."};
static const reply ERR_NEEDMOREPARAMS = {"461", "Not enough parameters"};
static const reply ERR_CHANNELISFULL = {"471", "Cannot join channel (+l)"};
static const reply ERR_INVITEONLYCHAN = {"473", "Cannot join channel (+i)"};
static const reply ERR_BADCHANNELKEY = {"475", "Cannot join channel (+k)"};
static const reply ERR_BADCHANMASK = {"476", "Bad Channel Mask"};
static const reply ERR_CHANOPRIVSNEEDED = {"482", "You're not channel operator"};