include ../config.mk

top_builddir = ..
INSTALL = $(top_builddir)/install-sh -c

CPPFLAGS = $(BASECPPFLAGS) -I../ -I../include

.SUFFIXES: .lo

all: svr lsnr inetclnt unixclnt dmn err cmdline log passwdgroup permissions protohost regexp sem2 shm2 text datetime dtd mem random sem1 shm1 signal string handoff1 handoff2 handoffclient nonhandoff1 nonhandoff2 nonhandoffclient vbuf xmls xmld

clean:
	$(LIBTOOL) --mode=clean $(RM) *.lo
	$(LIBTOOL) --mode=clean $(RM) *.o
	$(LIBTOOL) --mode=clean $(RM) lsnr svr inetclnt unixclnt dmn err cmdline log passwdgroup permissions protohost regexp sem1 sem2 shm1 shm2 text datetime dtd mem random signal string handoff1 handoff2 handoffclient nonhandoff1 nonhandoff2 nonhandoffclient vbuf xmls xmld
	$(RM) test.log

rebuild: clean all

.C.lo:
	$(LIBTOOL) --mode=compile $(CXX) $(CXXFLAGS) $(CPPFLAGS) -g -c $< -o $@

svr: svr.C svr.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ svr.lo -L../src -lrudiments_p $(SOCKETLIB) $(NSLLIB)

lsnr: lsnr.C lsnr.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ lsnr.lo -L../src -lrudiments_p $(SOCKETLIB) $(NSLLIB)

inetclnt: inetclnt.C inetclnt.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ inetclnt.lo -L../src -lrudiments_p $(SOCKETLIB) $(NSLLIB)

unixclnt: unixclnt.C unixclnt.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ unixclnt.lo -L../src -lrudiments_p $(SOCKETLIB) $(NSLLIB)

dmn: dmn.C dmn.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ dmn.lo -L../src -lrudiments_p

err: err.C err.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ err.lo -L../src -lrudiments_p

cmdline: cmdline.C cmdline.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ cmdline.lo -L../src -lrudiments_p

log: log.C log.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ log.lo -L../src -lrudiments_p

mem: mem.C mem.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ mem.lo -L../src -lrudiments_p

passwdgroup: passwdgroup.C passwdgroup.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ passwdgroup.lo -L../src -lrudiments_p

permissions: permissions.C permissions.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ permissions.lo -L../src -lrudiments_p

protohost: protohost.C protohost.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ protohost.lo -L../src -lrudiments_p

regexp: regexp.C regexp.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ regexp.lo -L../src -lrudiments_p

sem1: sem1.C sem1.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ sem1.lo -L../src -lrudiments_p

sem2: sem2.C sem2.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ sem2.lo -L../src -lrudiments_p

shm1: shm1.C shm1.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ shm1.lo -L../src -lrudiments_p

shm2: shm2.C shm2.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ shm2.lo -L../src -lrudiments_p

text: text.C text.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ text.lo -L../src -lrudiments_p

datetime: datetime.C datetime.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ datetime.lo -L../src -lrudiments_p

dtd: dtd.C dtd.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ dtd.lo -L../src -lrudiments_p

random: random.C random.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ random.lo -L../src -lrudiments_p

signal: signal.C signal.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ signal.lo -L../src -lrudiments_p

string: string.C string.lo 
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ string.lo -L../src -lrudiments_p -lefence

handoff1: handoff1.C handoff1.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ handoff1.lo -L../src -lrudiments_p $(SOCKETLIB) $(NSLLIB)

handoff2: handoff2.C handoff2.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ handoff2.lo -L../src -lrudiments_p $(SOCKETLIB) $(NSLLIB)

handoffclient: handoffclient.C handoffclient.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ handoffclient.lo -L../src -lrudiments_p $(SOCKETLIB) $(NSLLIB)

nonhandoff1: nonhandoff1.C nonhandoff1.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ nonhandoff1.lo -L../src -lrudiments_p $(SOCKETLIB) $(NSLLIB)

nonhandoff2: nonhandoff2.C nonhandoff2.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ nonhandoff2.lo -L../src -lrudiments_p $(SOCKETLIB) $(NSLLIB)

nonhandoffclient: nonhandoffclient.C nonhandoffclient.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ nonhandoffclient.lo -L../src -lrudiments_p $(SOCKETLIB) $(NSLLIB)

vbuf: vbuf.C vbuf.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ vbuf.lo -L../src -lrudiments_p

xmls: xmls.C xmls.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ xmls.lo -L../src -lrudiments_p

xmld: xmld.C xmld.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ xmld.lo -L../src -lrudiments_p
