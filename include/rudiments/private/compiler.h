// Copyright (c) 2015 David Muse
// See the COPYING file for more information.

	private:
		bool	appendModule(
				const char *filename,
				const char *classname,
				const char *id,
				const char *textparams,
				xmldomnode *xmlparams,
				linkedlist< compilermodule * > *modulelist);

		void	clearModules(
				linkedlist< compilermodule * > *modulelist);

		compilerprivate	*pvt;
