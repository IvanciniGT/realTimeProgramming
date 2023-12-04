
# Project structure

mainFolder/
    globalSystemProcess/        Is going to integrate everything
        main.c
    cardValidator/ -> pointing to another git repository (at a certain version- we usually use tags to identify those versions)
        main.c
    cardDetector/
    logger/
    heartbeat/
    common_signals/             turn_on, turn_off, shutdown
    and more subprocess and shared libraries folders

How are we going to store all these files in git?

# Git

Inside git, we will create a repository for this project (mainFolder/)
This repository is not going to contain all those folders-

A mainSystem... this main System, is going to be composed of several subsystems / subprocess.

Maybe we sill have a mainSystem1 and a mainSystem2. For each of them we will have a repository.
Maybe my mainSystem1 is going to have just 1 single cardDetector, and my mainSystem2 is going to have 2 cardDetectors.

What we will do in git is to make use of git submodules.
Git submodules allow me to define folders inside my repository that are going to be pointing to other repositories.
In the main system repository each commit is going to have information about the commit that we will use of each submodule.

git submodule add <url> <path>

git submodule init

// When cloning a repository with submodules, we will need to do:
git clone --recurse-submodules <url>

// Case we want to upgrade the version of a submodule:
access the submodule folder
git pull origin
git checkout <tag>

back in the main repository folder
git add <submodule>
git commit -m "Upgrading submodule version"
git push origin

## Commits in git

Most people believe that a commit is a set of changes that we have done in our code.
A commit has an ID which actually is a HASH of the content of the commit.

A commit is a full copy of all the files in the project.

Other scm systems, like svn os cvs, they store the difference between the previous commit and the current commit.
Git does not do this... it stores a full copy of the project.
Whenever we need to check the difference between 2 commits, git will calculate the difference between those 2 commits.

