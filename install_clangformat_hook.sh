#!/bin/bash

cp clangFormatHook/.clang-format .clang-format
if [ -d .git ]; then \
    echo ".git is a folder - copying files to .git/hooks directory"; \
    cp clangFormatHook/pre-commit .git/hooks/; \
    chmod +x .git/hooks/pre-commit; \
elif [ -f .git ]; then \
    echo ".git is a file - copying files to root .git/hooks directory"; \
    cp clangFormatHook/pre-commit $(GITPATH)/hooks/; \
    chmod +x $(GITPATH)/hooks/pre-commit; \
else \
    echo "no valid .git directory or file found!"; \
fi