#pragma once
#include <string>

struct reply {
    std::string code;
    std::string msg;
};

static const reply RPL_WELCOME = {"001", " :Welcome to IRC\r\n"};
static const reply RPL_YOURHOST = {"002", " :Your host is ircserv\r\n"}; 
static const reply RPL_CREATED = {"003", " :This server was created today\r\n"};
static const reply RPL_MYINFO = {"004", " ircserv 1.0 o o\r\n"}; 
static const reply RPL_ISUPPORT = {"005", " CHANTYPES=# NICKLEN=9 PREFIX=(ov)@+ CHANMODES=itkol :are supported by this server\r\n"};
//static const err RPL_TOPIC = {"332"};
//static const err RPL_TOPICWHOTIME  = {"333"};
//static const err RPL_NAMREPLY = {"353"};
static const reply RPL_ENDOFNAMES = {"366", " :End of /NAMES list\r\n"};
static const reply ERR_NOSUCHNICK = {"401", " :No such nick/channel\r\n"};
static const reply ERR_CANNOTSENDTOCHAN = {"404", " :Cannot send to channel\r\n"};
static const reply ERR_NOSUCHCHANNEL = {"403",  " :No such channel\r\n"};
static const reply ERR_TOOMANYCHANNELS = {"405", " :You have joined too many channels\r\n"};
static const reply ERR_NICKNAMEINUSE = {"433", " :Nickname is already in use\r\n"};
static const reply ERR_NEEDMOREPARAMS = {"461", " :Not enough parameters\r\n"};
static const reply ERR_CHANNELISFULL = {"471", " :Cannot join channel (+l)\r\n"};
static const reply ERR_INVITEONLYCHAN = {"473", " :Cannot join channel (+i)\r\n"};
static const reply ERR_BADCHANNELKEY = {"475", " :Cannot join channel (+k)\r\n"};
static const reply ERR_BADCHANMASK = {"476", " :Bad Channel Mask\r\n"};