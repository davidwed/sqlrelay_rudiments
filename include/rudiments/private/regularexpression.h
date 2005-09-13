// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

	private:
		void	regularexpressionInit();
		// FIXME: it should be possible to copy a regularexpression,
		// but how do you copy *expr and *extra?
				regularexpression(const regularexpression &r);
		regularexpression	&operator=(const regularexpression &r);

		regularexpressionprivate	*pvt;
