#Committing changes to SDL

We would like to make it easier for community members to contribute to SDL
using pull requests. This makes the process of contributing a little easier for the contributor since they don't
need to concern themselves with the question, "What branch do I base my changes
on?"  This is already called out in the CONTRIBUTING.md.

##Terminology
Many of these terms have more than one meaning. For the purposes of this
document, the following terms refer to specific things.

**contributor** - A person who makes a change to SDL and submits a change
set in the form of a pull request.

**reviewer** - A person responsible for reviewing a pull request.

**master branch** - [The base branch](https://github.com/smartdevicelink/sdl_core/tree/master).

**develop branch** - [The branch](https://github.com/LuxoftSDL/sdl_core/tree/develop) where bug fixes against the latest release or release candidate are merged.

## Pre-commit hook installation
* Go to tools/infrastructure/
* Run install-hooks.py
* or : Just run "python tools/infrastructure/install-hooks.py"

##Pull request checklist
* Add Unit tests.
* All tests pass (see Run all tests section).
* Check component design
* Check amount of required memory, memory leaks.
* Assertion must be used in the right way.
* Are there cyclic dependencies?
* Level of abstraction adequate?
* Are there any platform gotchas? (Does a change make an assumption about
   platform specific behavior that is incompatible with other platforms?  e.g.
   Windows paths vs. POSIX paths).
* Thread safe code.
* No deadlocks.
* Is Doxygen API documentation available?
* Do you have thread specific comment provided? (if another thread uses added method/classes, 
   there must be comment which explains: When method will be called by thread? Which thread calls the method? e.g.)
* Do you have specific realization code comments?
* Add log messages.
* There are no Google code style errors. (You can download Google [cpplint.py](https://raw.githubusercontent.com/google/styleguide/gh-pages/cpplint/cpplint.py))
* Check branch naming.
* Check correct commit messages in commits (see Pull request message section).
* Check correct pull request target (`master` or `develop`)
* Check correct pull request message
* Add reviewers

##Run all tests
* Run in root build directory `make test`

##Branch naming
Branch name should be:
* `hotfix/<task name>` or
* `feature/<task name>` or
* `fix/<task name>`

##Pull request message
* Describe the reason why you created pull request and what changes there are.
* Add related Jira ticket as link.
   ( EXAMPLE:
   `Related: [APPLINK-xxxxx](put direct link here)`)
* If there was an old pull request, add link. ( EXAMPLE: `Old pull request is [here](put direct link here)`)

##Adding reviewers:
* Add one domain expert
* Add your mentor (for junior developers only)
* Add all junior developers (for junior developers only)

`Note`: Everyone from SDL team developers can review any pull request (“reviewed” comment is not required)

##Review process
* Reviewers can leave comments to code only in `commits` tab.
* Contributor must answer each comment (Contributor should specify addressee in comment).
* In case contributor is disagree with reviewer, contributor writes his opinion
* In case contributor is agree with reviewer, contributor leaves link to new commit with changes.
* When reviewer is agree with all changes in pull request, he must leave `Reviewed` comment.

##Successfully passed review:
* Contributor answered to all comments
* Contributor fixed all mistakes
* All reviewers left `Reviewed` comment

##Merging
* Successfully passed review.
* Rebase in case of existing conflicts.
* Contributor can squash commits in case of adding same code in different commits.
* Contact @AGaliuzov or @anosach-luxoft to merge pull request.

##Additional sources
* [Google cppint.py](https://raw.githubusercontent.com/google/styleguide/gh-pages/cpplint/cpplint.py)
* [Git commit best practices](http://chris.beams.io/posts/git-commit/)
* [GitHub Working with formatting](https://help.github.com/articles/working-with-advanced-formatting/)
