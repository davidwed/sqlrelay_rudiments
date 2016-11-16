top_builddir = ..

include ../config.mk

CPPFLAGS = $(BASECPPFLAGS) $(INC)../ $(INC)../include -std=c++11
LIBS=$(RUDIMENTSLIB) $(SOCKETLIBS) $(PTHREADLIB) $(READLINELIBS) $(SSLLIBS) $(GSSLIBS) $(PCRELIBS) $(LIBCURLLIBS) $(NANOSLEEPLIB) $(CLOCKNANOSLEEPLIB) $(CRYPTLIB) $(INETATONLIB) $(BELIB) $(GNULIB) $(CRTLIB) $(DLLIB) $(GETPGIDLIB) $(DEBUGLIB)

PROGS = dictionary \
	dynamicarray \
	forward_list \
	linkedlist \
	list \
	map \
	singlylinkedlist \
	stringbuffer \
	string \
	vector

.SUFFIXES: .lo .obj

.cpp.lo:
	$(LTCOMPILE) $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(COMPILE) $< $(OUT)$@

.cpp.obj:
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(COMPILE) $<

all: $(PROGS)

clean:
	$(LTCLEAN) $(RM) $(PROGS)
	$(LTCLEAN) $(RM) *.exe *.ilk *.idb *.pdb *.manifest *.lo *.o *.obj

dictionary: dictionary.cpp dictionary.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) dictionary.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

dynamicarray: dynamicarray.cpp dynamicarray.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) dynamicarray.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

forward_list: forward_list.cpp forward_list.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) forward_list.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

linkedlist: linkedlist.cpp linkedlist.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) linkedlist.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

list: list.cpp list.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) list.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

map: map.cpp map.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) map.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

singlylinkedlist: singlylinkedlist.cpp singlylinkedlist.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) singlylinkedlist.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

stringbuffer: stringbuffer.cpp stringbuffer.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) stringbuffer.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

string: string.cpp string.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) string.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

vector: vector.cpp vector.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) vector.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)
