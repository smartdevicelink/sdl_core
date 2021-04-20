# Contributing to SDL Projects

Third party contributions are essential for making SDL great. However, we do have a few guidelines we need contributors to follow.

## Environment
Currently supported:
* Ubuntu Linux 16.04 with GCC 5.4.x
* Ubuntu Linux 18.04 with GCC 7.3.x
* Ubuntu Linux 20.04 with GCC 9.3.x
* [C++11 standard](https://github.com/smartdevicelink/sdl_evolution/issues/132)

### Issues
If writing a bug report, please make sure [you follow the issue template](https://github.com/smartdevicelink/sdl_core/blob/master/.github/ISSUE_TEMPLATE.md). Include all relevant information.

If requesting a feature, understand that we appreciate the input! However, it may not immediately fit our roadmap, and it may take a while for us to get to your request.

### Gitflow
We use [GitFlow](http://nvie.com/posts/a-successful-git-branching-model/) as our branch management system. Please follow GitFlow's guidelines while contributing to any SDL project.

### Pull Requests
* Please follow the repository's [Style Guide](https://github.com/smartdevicelink/sdl_core/wiki/SDL-Coding-Style-Guide) for all code and documentation.
* All pull requests should be sent to `smartdevicelink/sdl_core/`, to `develop` or `master` branch.
* All feature branches should be based on `develop` and have the format `feature/branch_name`.
* All fix branches should be based on `develop` and have the format `fix/branch_name`.
* All new functionality requests should be provided only for `develop` branch.
* In case an issue should be fixed in a short time (after release), open a pull request to `master` with a branch name of `hotfix/branch_name`. 
* In case an issue exists in both the `develop` and `master` branches, open a pull request to `develop` only. Do not open the same pull request against the `master` branch. 
* All pull requests should implement a single feature or fix a single bug. Pull Requests that involve multiple changes (it is our discretion what precisely this means) will be rejected with a reason.
* All commits should be separated into logical units, i.e. unrelated changes should be in different commits within a pull request.
* Work in progress pull requests should have "[WIP]" in front of the Pull Request title. When you believe the pull request is ready to merge, remove this tag and @mention the appropriate SDL team to schedule a review.
* All new code *must* include unit tests. Bug fixes should have a test that fails previously and now passes. All new features should be covered. If your code does not have tests, or regresses old tests, it will be rejected.
* A great example of a [pull request can be found here](https://github.com/smartdevicelink/SmartDeviceLink-iOS/pull/45).

### Contributor's License Agreement (CLA)
In order to accept Pull Requests from contributors, you must first sign [the Contributor's License Agreement](https://docs.google.com/forms/d/1VNR8EUd5b46cQ7uNbCq1fJmnu0askNpUp5dudLKRGpU/viewform). If you need to make a change to information that you entered, [please contact us](mailto:theresa@livio.io).

## Additional Resources
* [General GitHub documentation](https://help.github.com/)
* [GitHub pull request documentation](https://help.github.com/send-pull-requests/)
