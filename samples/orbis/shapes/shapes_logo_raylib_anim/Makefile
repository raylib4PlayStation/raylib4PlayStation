ifndef ORBISDEV
$(error ORBISDEV, is not set)
endif

target ?= ps4_elf_sce
TargetFile=homebrew.elf

include $(ORBISDEV)/make/ps4sdk.mk
#CompilerFlagsCpp +=-D__DEBUGNET_PC_IP__=\"$(DEBUGNET_PC_IP)\" 
LinkerFlags+= -luser_mem_sys -lkernel_stub  -lSceLibcInternal_stub -lraylib -lorbisLink -lkernelUtil -ldebugnet -lorbisNfs -lSceSysmodule_stub -lSceSystemService_stub -lSceNet_stub -lSceUserService_stub -lScePigletv2VSH_stub -lSceVideoOut_stub -lSceGnmDriver_stub  -lorbisPad -lScePad_stub -lSceAudioOut_stub -lSceIme_stub -lSceNetCtl_stub -lSQLite
IncludePath += -I$(ORBISDEV)/usr/include/orbis
#-I$(ORBISDEV)/usr/include -I$(ORBISDEV)/usr/include/c++/v1 -I$(ORBISDEV)/usr/include/orbis
AUTH_INFO = 000000000000000000000000001C004000FF000000000080000000000000000000000000000000000000008000400040000000000000008000000000000000080040FFFF000000F000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000

install:
	@cp $(OutPath)/homebrew.self $(SELF_PATH_INSTALL)
	@echo "Installed!"
oelf:
	orbis-elf-create bin/homebrew.elf bin/homebrew.oelf
eboot:
	python2.7 $(ORBISDEV)/bin/make_fself.py --auth-info $(AUTH_INFO) bin/homebrew.oelf bin/homebrew.self
pkg_build:
	cp bin/homebrew.self pkg/eboot.bin && cd pkg && pkgTool pkg_build Project.gp4 . && cp *.pkg ../bin/
