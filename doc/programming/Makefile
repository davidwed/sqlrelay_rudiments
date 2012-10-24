top_builddir = ../..

include ../../config.mk

html:
	cd parts; \
	rm -f *.html; \
	for i in `/bin/ls ../../../examples/*.cpp`; \
	do \
		BASENAME=`/bin/basename $$i`; \
		echo "processing $$BASENAME"; \
		gvim $$i +":runtime! syntax/2html.vim" +"colorscheme default" +":x! ./$$BASENAME.html" +"q!"; \
		sleep 2; \
	done
	for i in `/bin/ls parts/*.html`; \
	do \
		sed -i -e "s/<html>//" -e "s/<head>//" -e "s/<title>.*<\/title>//" -e "s/<meta.*//" -e "s/<\/head>//" -e "s/<body.*//" -e "s/<\/body>//" -e "s/<\/html>//" $$i; \
	done
	./insert.pl baseclasses.html.in > baseclasses.html
	./insert.pl utilclasses.html.in > utilclasses.html
