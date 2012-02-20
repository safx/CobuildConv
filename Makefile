#
# Makefile
#
#
#

###########################

TARGET=conv
SOURCE=CobuildConv.cpp
COBUILD=/Volumes/COBUILD/hcp_en-cc5.trd

# You need to edit these values.

DICT_NAME		=	"COBUILD"
DICT_SRC_PATH	=	cobuild.xml
CSS_PATH		=	cobuild.css
PLIST_PATH		=	cobuild.plist

DICT_BUILD_OPTS		=
# Suppress adding supplementary key.
# DICT_BUILD_OPTS		=	-s 0	# Suppress adding supplementary key.

###########################

# The DICT_BUILD_TOOL_DIR value is used also in "build_dict.sh" script.
# You need to set it when you invoke the script directly.

DICT_BUILD_TOOL_DIR	=	"/Developer/Extras/Dictionary Development Kit"
DICT_BUILD_TOOL_BIN	=	"$(DICT_BUILD_TOOL_DIR)/bin"

###########################

DICT_DEV_KIT_OBJ_DIR	=	./objects
export	DICT_DEV_KIT_OBJ_DIR

DESTINATION_FOLDER	=	~/Library/Dictionaries
RM			=	/bin/rm

###########################

$(TARGET): $(SOURCE)
	$(CXX) -g -o $(TARGET) $(SOURCE)

$(DICT_SRC_PATH): $(TARGET)
	 ./$(TARGET) $(COBUILD)

dic: $(DICT_SRC_PATH)
	"$(DICT_BUILD_TOOL_BIN)/build_dict.sh" $(DICT_BUILD_OPTS) $(DICT_NAME) $(DICT_SRC_PATH) $(CSS_PATH) $(PLIST_PATH)
	echo "Done."

install:
	echo "Installing into $(DESTINATION_FOLDER)".
	mkdir -p $(DESTINATION_FOLDER)
	ditto --noextattr --norsrc $(DICT_DEV_KIT_OBJ_DIR)/$(DICT_NAME).dictionary  $(DESTINATION_FOLDER)/$(DICT_NAME).dictionary
	touch $(DESTINATION_FOLDER)
	echo "Done."
	echo "To test the new dictionary, try Dictionary.app."

clean:
	$(RM) -rf $(DICT_DEV_KIT_OBJ_DIR)
	rm -f $(TARGET)
	rm -fr $(TARGET).dSYM
	rm -f $(WORKDIR)
	rm -f $(DICT_SRC_PATH)