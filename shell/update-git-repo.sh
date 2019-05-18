#!/bin/bash

function git_update {
    cd $1
    # 0: check whether a git repository 
    if git rev-parse --git-dir >/dev/null 2>&1; then
        # Git repository
        #echo "$0: Git repository: $1"

        # 1: stash if required
        changes="false"
        if git diff-index --quiet HEAD -- >/dev/null; then
            # No changes
            echo "$0: No changes in $1"
        else
            # Changes
            echo "$0: Stash changes in $1"
            changes="true"
            git stash >/dev/null
        fi

        # 2: checkout to master if required
        current_branch=$(git rev-parse --abbrev-ref HEAD)
        if [ $current_branch != "master" ]; then
            echo "$0: Checkout from $current_branch to master"
            git checkout master >/dev/null
        fi

        # 3: update repository
        git pull  >/dev/null 2>&1

        # 4: restore branch if required
        if [ $current_branch != "master" ]; then
            echo "$0: Checkout from master to $current_branch" 
            git checkout $current_branch >/dev/null
        fi

        # 5: unstash if required
        if [ $changes == "true" ]; then
            git stash pop >/dev/null
        fi

    else
        echo "$0: Not a git repository: $1"
    fi
}

function main {
    git_repos=$(find $1 -name '.git' | xargs -n 1 dirname)
    #echo $git_repos

    for d in $git_repos; do
        #echo "$0: Git repo: $d"
        git_update $d
    done
}

main $1
