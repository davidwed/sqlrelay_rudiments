// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef CLIENTTRANSPORT_H
#define CLIENTTRANSPORT_H

#include <rudiments/transport.h>
#include <rudiments/connectiondata.h>

class clienttransport : virtual public transport {
	public:
		virtual void	initialize(connectiondata *cd)=0;
		virtual int	connect()=0;
	protected:
		unsigned int	retrywait;
		int		retrycount;
};

#endif
