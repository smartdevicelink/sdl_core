# Contributing to SDL Projects

Third party contributions are essential for making SDL great. However, we do have a few guidelines we need contributors to follow.

### Issues
If writing a bug report, please make sure <a href="http://yourbugreportneedsmore.info" target="_blank">it has enough info</a>. Include all relevant information.

If requesting a feature, understand that we appreciate the input! However, it may not immediately fit our roadmap, and it may take a while for us to get to your request.

### Gitflow
We use <a href="http://nvie.com/posts/a-successful-git-branching-model/">Gitflow</a> as our branch management system. Please follow gitflow's guidelines while contributing to any SDL project.

### Pull Requests
* Please follow the repository's for all code and documentation.
* All pull requests should be sent to `smartdevicelink/sdl_core/`, to `develop` or `master` branch.
* All feature branches should be based on `develop` and have the format `feature/branch_name`.
* All fix branches should be based on `develop` and have the format `fix/branch_name`.
* All new functionality requests should be provided only for `develop` branch.
* In case defect should be fixed in short time (after release), send pull request to `master` and have the format `hotfix/branch_name`. 
* In case defect exists in `develop` and `master` branches, send pull request to `develop` only. Do not send the same pull request to the `master` branch. 
* All pull requests should implement a single feature or fix a single bug. Pull Requests that involve multiple changes (it is our discretion what precisely this means) will be rejected with a reason.
* All commits should be separated into logical units, i.e. unrelated changes should be in different commits within a pull request.
* Work in progress pull requests should have "[WIP]" in front of the Pull Request title. When you believe the pull request is ready to merge, remove this tag and @mention the appropriate SDL team to schedule a review.
* All new code *must* include unit tests. Bug fixes should have a test that fails previously and now passes. All new features should be covered. If your code does not have tests, or regresses old tests, it will be rejected.
* A great example of a [pull request can be found here](https://github.com/smartdevicelink/SmartDeviceLink-iOS/pull/45).

### Contributor's License Agreement (CLA)
In order to accept Pull Requests from contributors, you must first sign [the Contributor's License Agreement](https://docs.google.com/forms/d/1VNR8EUd5b46cQ7uNbCq1fJmnu0askNpUp5dudLKRGpU/viewform). If you need to make a change to information that you entered, [please contact us](mailto:justin@livio.io).

## Additional Resources
* [General GitHub documentation](https://help.github.com/)
* [GitHub pull request documentation](https://help.github.com/send-pull-requests/)
* [Contributor's License Agreement](https://docs.google.com/forms/d/1VNR8EUd5b46cQ7uNbCq1fJmnu0askNpUp5dudLKRGpU/viewform)
* [Committers.md](https://github.com/LuxoftSDL/sdl_core/blob/feature/Add_Committers_file/COMMITTERS.md)

