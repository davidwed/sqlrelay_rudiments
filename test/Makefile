include ../config.mk

top_builddir = ..
INSTALL = $(top_builddir)/install-sh -c

CPPFLAGS = $(BASECPPFLAGS) -I../ -I../include

.SUFFIXES: .lo

all: svr lsnr inetclnt unixclnt dmn err group cmdline paramstr host log oneshotclnt oneshotsvr passwd service shadow permissions protocol regexp rpc sem2 shm2 text datetime tzf dict dtd env file filesystem list lock1 lock2 mem random sem1 shm1 signal string strbuf handoff1 handoff2 handoffclient nonhandoff1 nonhandoff2 nonhandoffclient vbuf xmls xmld

clean:
	$(LIBTOOL) --mode=clean $(RM) *.lo
	$(LIBTOOL) --mode=clean $(RM) *.o
	$(LIBTOOL) --mode=clean $(RM) lsnr svr inetclnt unixclnt dmn err group cmdline paramstr host log oneshotclnt oneshotsvr passwd service shadow permissions protocol regexp rpc sem1 sem2 shm1 shm2 text datetime tzf dict dtd env file filesystem list lock1 lock2 mem random signal string strbuf handoff1 handoff2 handoffclient nonhandoff1 nonhandoff2 nonhandoffclient vbuf xmls xmld
	$(RM) test.log
	$(RMTREE) .libs
	$(RM) testfile

rebuild: clean all

.C.lo:
	$(LIBTOOL) --mode=compile $(CXX) $(CXXFLAGS) $(CPPFLAGS) -g -c $< -o $@

svr: svr.C svr.lo 
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ svr.lo -L../src -lrudiments $(SOCKETLIB) $(NSLLIB) $(PTHREADLIB)

lsnr: lsnr.C lsnr.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ lsnr.lo -L../src -lrudiments $(SOCKETLIB) $(NSLLIB) $(PTHREADLIB)

inetclnt: inetclnt.C inetclnt.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ inetclnt.lo -L../src -lrudiments $(SOCKETLIB) $(NSLLIB) $(PTHREADLIB)

unixclnt: unixclnt.C unixclnt.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ unixclnt.lo -L../src -lrudiments $(SOCKETLIB) $(NSLLIB) $(PTHREADLIB)

dmn: dmn.C dmn.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ dmn.lo -L../src -lrudiments $(PTHREADLIB)

err: err.C err.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ err.lo -L../src -lrudiments $(PTHREADLIB)

cmdline: cmdline.C cmdline.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ cmdline.lo -L../src -lrudiments $(PTHREADLIB)

paramstr: paramstr.C paramstr.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ paramstr.lo -L../src -lrudiments $(PTHREADLIB)

log: log.C log.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ log.lo -L../src -lrudiments $(PTHREADLIB)

env: env.C env.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ env.lo -L../src -lrudiments $(PTHREADLIB)

file: file.C file.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ file.lo -L../src -lrudiments $(PTHREADLIB)

filesystem: filesystem.C filesystem.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ filesystem.lo -L../src -lrudiments $(PTHREADLIB)

group: group.C group.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ group.lo -L../src -lrudiments $(PTHREADLIB)

host: host.C host.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ host.lo -L../src -lrudiments $(PTHREADLIB)

list: list.C list.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ list.lo -L../src -lrudiments $(PTHREADLIB)

lock1: lock1.C lock1.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ lock1.lo -L../src -lrudiments $(PTHREADLIB)

lock2: lock2.C lock2.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ lock2.lo -L../src -lrudiments $(PTHREADLIB)

mem: mem.C mem.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ mem.lo -L../src -lrudiments $(PTHREADLIB)

oneshotclnt: oneshotclnt.C oneshotclnt.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ oneshotclnt.lo -L../src -lrudiments $(PTHREADLIB)

oneshotsvr: oneshotsvr.C oneshotsvr.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ oneshotsvr.lo -L../src -lrudiments $(PTHREADLIB)

passwd: passwd.C passwd.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ passwd.lo -L../src -lrudiments $(PTHREADLIB)

service: service.C service.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ service.lo -L../src -lrudiments $(PTHREADLIB)

shadow: shadow.C shadow.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ shadow.lo -L../src -lrudiments $(PTHREADLIB)

permissions: permissions.C permissions.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ permissions.lo -L../src -lrudiments $(PTHREADLIB)

protocol: protocol.C protocol.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ protocol.lo -L../src -lrudiments $(PTHREADLIB)

regexp: regexp.C regexp.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ regexp.lo -L../src -lrudiments $(PTHREADLIB)

rpc: rpc.C rpc.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ rpc.lo -L../src -lrudiments $(PTHREADLIB)

sem1: sem1.C sem1.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ sem1.lo -L../src -lrudiments $(PTHREADLIB)

sem2: sem2.C sem2.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ sem2.lo -L../src -lrudiments $(PTHREADLIB)

shm1: shm1.C shm1.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ shm1.lo -L../src -lrudiments $(PTHREADLIB)

shm2: shm2.C shm2.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ shm2.lo -L../src -lrudiments $(PTHREADLIB)

text: text.C text.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ text.lo -L../src -lrudiments $(PTHREADLIB)

datetime: datetime.C datetime.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ datetime.lo -L../src -lrudiments $(PTHREADLIB)

tzf: tzf.C tzf.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ tzf.lo -L../src -lrudiments $(PTHREADLIB)

dict: dict.C dict.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ dict.lo -L../src -lrudiments $(PTHREADLIB)

dtd: dtd.C dtd.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ dtd.lo -L../src -lrudiments $(PTHREADLIB)

random: random.C random.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ random.lo -L../src -lrudiments $(PTHREADLIB)

signal: signal.C signal.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ signal.lo -L../src -lrudiments $(PTHREADLIB)

string: string.C string.lo 
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ string.lo -L../src -lrudiments $(PTHREADLIB)

strbuf: strbuf.C strbuf.lo 
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ strbuf.lo -L../src -lrudiments $(PTHREADLIB)

handoff1: handoff1.C handoff1.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ handoff1.lo -L../src -lrudiments $(SOCKETLIB) $(NSLLIB) $(PTHREADLIB)

handoff2: handoff2.C handoff2.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ handoff2.lo -L../src -lrudiments $(SOCKETLIB) $(NSLLIB) $(PTHREADLIB)

handoffclient: handoffclient.C handoffclient.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ handoffclient.lo -L../src -lrudiments $(SOCKETLIB) $(NSLLIB) $(PTHREADLIB)

nonhandoff1: nonhandoff1.C nonhandoff1.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ nonhandoff1.lo -L../src -lrudiments $(SOCKETLIB) $(NSLLIB) $(PTHREADLIB)

nonhandoff2: nonhandoff2.C nonhandoff2.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ nonhandoff2.lo -L../src -lrudiments $(SOCKETLIB) $(NSLLIB) $(PTHREADLIB)

nonhandoffclient: nonhandoffclient.C nonhandoffclient.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ nonhandoffclient.lo -L../src -lrudiments $(SOCKETLIB) $(NSLLIB) $(PTHREADLIB)

vbuf: vbuf.C vbuf.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ vbuf.lo -L../src -lrudiments $(PTHREADLIB)

xmls: xmls.C xmls.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ xmls.lo -L../src -lrudiments $(PTHREADLIB)

xmld: xmld.C xmld.lo
	$(LIBTOOL) --mode=link $(CXX) -all-static -o $@ xmld.lo -L../src -lrudiments $(PTHREADLIB)
