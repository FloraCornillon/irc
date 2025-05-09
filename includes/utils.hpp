#pragma once

//USEFUL ERROR CODE

#define ERR_NOSUCHNICK			"401 No such nick/channel"			// 401
#define ERR_NOSUCHSERVER		"402 No such server"				// 402
#define ERR_NOSUCHCHANNEL		"403 No such channel"				// 403
#define ERR_CANNOTSENDTOCHAN	"404 Cannot send to channel"		// 404
#define ERR_TOOMANYCHANNELS		"405 Too many channels"				// 405
#define ERR_WILDTOPLEVEL		"406 Wild card in toplevel domain"	// 406
#define ERR_NORECIPIENT			"411 No recipient given"
#define ERR_NOTONCHANNEL		"442 Not on channel"				// 442
#define ERR_USERONCHANNEL		"443 User on channel"				// 443
#define ERR_NOORIGIN			"409 No origin"						// 409
#define ERR_UNKNOWNCOMMAND		"421 Unknown command"				// 421
#define ERR_NOMOTD				"422 No MOTD"						// 422
#define ERR_NOADMININFO			"423 No administrative info"		// 423
#define ERR_FILEERROR			"424 File error"					// 424
#define ERR_NONICKNAMEGIVEN		"431 No nickname given"				// 431
#define ERR_ERRONEUSNICKNAME	"432 Erroneous nickname"			// 432
#define ERR_NICKNAMEINUSE		"433 Nickname in use"				// 433
#define ERR_USERNOTFOUND		"441 User not found"				// 441
#define ERR_USERNOTINCHANNEL	"441 User not in channel"	
#define ERR_NOTREGISTERED		"451 You are not registered"		// 451
#define ERR_NEEDMOREPARAMS		"461 Not enough parameters"			// 461
#define ERR_ALREADYREGISTERED	"462 You are already registered"	// 462
#define ERR_PASSWDMISMATCH		"464 Password mismatch"				// 464
#define ERR_YOUREBANNEDCREEP	"465 You're banned"					// 465
#define ERR_CHANNELISFULL		"471 Channel is full"				// 471
#define ERR_UNKNOWNMODE			"472 Unknown mode"					// 472
#define ERR_INVITEONLYCHAN		"473 Invite only channel"			// 473
#define ERR_BANNEDFROMCHAN		"474 Banned from channel"			// 474
#define ERR_BADCHANNELKEY		"475 Bad channel key"				// 475
#define ERR_BADCHANMASK			"476 Bad channel mask"				// 476
#define ERR_NOCHANMODES			"477 Channel doesn't support modes"	// 477
#define ERR_CHANOPRIVSNEEDED	"482 You're not channel operator"	// 482
#define ERR_CANTKILLSERVER		"483 Can't kill server"				// 483
#define ERR_NOOPERHOST			"491 No oper host"					// 491
#define ERR_UMODEUNKNOWNFLAG	"501 Unknown mode flag"				// 501
#define ERR_USERSDONTMATCH		"502 Users don't match"				// 502
#define ERR_NOTEXTTOSEND		"412 No text to send"				// 412

//USEFUL RESPONSE CODES
#define RPL_NOTOPIC				"331 No topic is set"				// 331
#define RPL_TOPIC				"332 Topic for channel"				// 332
#define RPL_NAMREPLY			"353 Names list"					// 353
#define RPL_ENDOFNAMES			"366 End of NAMES list"				// 366
#define RPL_YOUREOPER			"381 You are now an IRC operator"	// 381
#define RPL_INVITING			"341 User has been invited"			// 341
#define RPL_WHOREPLY			"352 Who reply"						// 352
#define RPL_ENDOFWHO			"315 End of WHO list"				// 315
#define RPL_LISTSTART			"321 Channel list start"			// 321
#define RPL_LIST				"322 Channel list"					// 322
#define RPL_LISTEND				"323 End of channel list"			// 323
#define RPL_CHANNELMODEIS		"<channel> <mode> <mode params>"	// 323


#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

#include <string>

std::vector<std::string>	split(const std::string& str, char delimiter);
std::string					trim(const std::string &str, const std::string &char_to_trim);

