/*       +------------------------------------+
 *       | Inspire Internet Relay Chat Daemon |
 *       +------------------------------------+
 *
 *  InspIRCd: (C) 2002-2008 InspIRCd Development Team
 * See: http://www.inspircd.org/wiki/index.php/Credits
 *
 * This program is free but copyrighted software; see
 *            the file COPYING for details.
 *
 * ---------------------------------------------------
 */

#include "inspircd.h"
#include "commands/cmd_whois.h"
#include "commands/cmd_stats.h"
#include "socket.h"
#include "wildcard.h"
#include "xline.h"
#include "transport.h"
#include "socketengine.h"

#include "m_spanningtree/main.h"
#include "m_spanningtree/utils.h"
#include "m_spanningtree/treeserver.h"
#include "m_spanningtree/link.h"
#include "m_spanningtree/treesocket.h"
#include "m_spanningtree/resolvers.h"
#include "m_spanningtree/handshaketimer.h"

/* $ModDep: m_spanningtree/timesynctimer.h m_spanningtree/resolvers.h m_spanningtree/main.h m_spanningtree/utils.h m_spanningtree/treeserver.h m_spanningtree/link.h m_spanningtree/treesocket.h */

/** remote ADMIN. leet, huh? */
bool TreeSocket::Admin(const std::string &prefix, std::deque<std::string> &params)
{
	if (params.size() > 0)
	{
		if (this->Instance->MatchText(this->Instance->Config->ServerName, params[0]))
		{
			/* It's for our server */
			string_list results;
			User* source = this->Instance->FindNick(prefix);
			if (source)
			{
				std::deque<std::string> par;
				par.push_back(prefix);
				par.push_back("");
				par[1] = std::string("::")+Instance->Config->ServerName+" 256 "+source->nick+" :Administrative info for "+Instance->Config->ServerName;
				Utils->DoOneToOne(this->Instance->Config->GetSID(), "PUSH",par, source->server);
				par[1] = std::string("::")+Instance->Config->ServerName+" 257 "+source->nick+" :Name     - "+Instance->Config->AdminName;
				Utils->DoOneToOne(this->Instance->Config->GetSID(), "PUSH",par, source->server);
				par[1] = std::string("::")+Instance->Config->ServerName+" 258 "+source->nick+" :Nickname - "+Instance->Config->AdminNick;
				Utils->DoOneToOne(this->Instance->Config->GetSID(), "PUSH",par, source->server);
				par[1] = std::string("::")+Instance->Config->ServerName+" 258 "+source->nick+" :E-Mail   - "+Instance->Config->AdminEmail;
				Utils->DoOneToOne(this->Instance->Config->GetSID(), "PUSH",par, source->server);
			}
		}
		else
		{
			/* Pass it on */
			User* source = this->Instance->FindNick(prefix);
			if (source)
				Utils->DoOneToOne(prefix, "ADMIN", params, params[0]);
		}
	}
	return true;
}
