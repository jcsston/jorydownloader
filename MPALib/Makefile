ifndef TARGET
	TARGET=./bin/Debug
else
	TARGET=./bin/$(TARGET)
endif

MCS=mcs
ifndef (RELEASE)
	MCSFLAGS=-debug --stacktrace
endif
LIBS=-lib:C:/PROGRA~1/MONO-1~1.2/lib\mono/1.0 -lib:C:/PROGRA~1/MONO-1~1.2/lib\mono/gtk-sharp


MPALIB_DLL=$(TARGET)/MPALib.dll
MPALIB_PDB=$(TARGET)/MPALib.pdb
MPALIB_SRC=AssemblyInfo.cs \
	Endian.cs \
	MPAFrame.cs \
	MPAReader.cs
MPALIB_RES=

$(MPALIB_DLL): $(MPALIB_SRC) 
	-mkdir -p $(TARGET)
	$(MCS) $(MCSFLAGS) $(LIBS) -r:System.dll -target:library -out:$(MPALIB_DLL) $(MPALIB_RES) $(MPALIB_SRC)


# common targets

all:	$(MPALIB_DLL)

clean:
	-rm -f "$(MPALIB_DLL)" 2> /dev/null
	-rm -f "$(MPALIB_PDB)" 2> /dev/null


# project names as targets

MPALib: $(MPALIB_DLL)
