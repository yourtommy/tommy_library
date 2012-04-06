SUBSYS=sort datastructure test

define \n


endef

# Usage: $(call MakeSub,SubDir,target)
# Note: The second line --the empty line
# is needed because this function
# is used inside a foreach statment.
# without it all function calls would
# be joined in a single line.
define MakeSubWithTarget
	$(foreach sub,${SUBSYS},$(MAKE) -C ${sub} $1${\n})
endef

define MakeSub
	$(call MakeSubWithTarget,$@)
endef

# The first target
release:
	$(call MakeSub)

all:
	$(call MakeSub)

debug:
	$(call MakeSub)

clean:
	$(call MakeSub)

