# Contributing to Sqlite3Utils

:+1::tada: First off, thanks for taking the time to contribute! :tada::+1:

The following is a set of guidelines for contributing to the Sqlite3Utils project on GitHub. These are mostly guidelines, not rules. Use your best judgment, and feel free to propose changes to this document in a pull request.

#### Table Of Contents

[I don't want to read this whole thing, I just have a question!!!](#i-dont-want-to-read-this-whole-thing-i-just-have-a-question)

[How Can I Contribute?](#how-can-i-contribute)
  * [Reporting Bugs](#reporting-bugs)
  * [Suggesting Enhancements](#suggesting-enhancements)
  * [Your First Code Contribution](#your-first-code-contribution)
  * [Pull Requests](#pull-requests)

[Additional Notes](#additional-notes)
  * [Issue and Pull Request Labels](#issue-and-pull-request-labels)


## I don't want to read this whole thing I just have a question!!!

> **Note:** Please don't file an issue to ask a question. You'll get faster results by using the resources below.

For any question you may have, please send them to our email :postbox:[sqlite3utils@hotmail.com](mailto:sqlite3utils@hotmail.com):postbox: and we will answer as soon as possible  :mailbox_with_no_mail: :inbox_tray: :mailbox_with_mail: :outbox_tray:

## How Can I Contribute?

### Reporting Bugs

This section guides you through submitting a bug report for Sqlite3Utils. Following these guidelines helps the developers understand your report :pencil:, reproduce the behavior :computer: :computer:, and find related reports :mag_right:.

Before creating bug reports, please check [this list](#before-submitting-a-bug-report) as you might find out that you don't need to create one. When you are creating a bug report, please [include as many details as possible](#how-do-i-submit-a-good-bug-report). Fill out [the required template](https://github.com/AEduardo-dev/Sqlite3Utils/blob/main/.github/ISSUE_TEMPLATE/bug_report.md) with the information it asks to helps us resolve issues faster.

> **Note:** If you find a **Closed** issue that seems like it is the same thing that you're experiencing, open a new issue and include a link to the original issue in the body of your new one.

#### Before Submitting A Bug Report
* **Search through the [project bug issues](https://github.com/AEduardo-dev/Sqlite3Utils/issues?q=is%3Aissue+label%3Abug)** to see if the problem has already been reported. If it has **and the issue is still open**, add a comment to the existing issue instead of opening a new one.

#### How Do I Submit A (Good) Bug Report?

Bugs are tracked as [GitHub issues](https://guides.github.com/features/issues/). After you've determined a clear description of the bug, please fill the [bug issue template](https://github.com/AEduardo-dev/Sqlite3Utils/blob/main/.github/ISSUE_TEMPLATE/bug_report.md) with all the information.

Explain the problem and include additional details to help maintainers reproduce the problem:

* **Use a clear and descriptive title** for the issue to identify the problem.
* **Describe the exact steps which reproduce the problem** in as many details as possible. When listing steps, **don't just say what you did, but explain how you did it**. For example, if you moved the cursor to the end of a line, explain if you used the mouse, or a keyboard shortcut, and if so which one?
* **Provide specific examples to demonstrate the steps**. Include links to files or GitHub projects, or copy/pasteable snippets, which you use in those examples. If you're providing snippets in the issue, use [Markdown code blocks](https://help.github.com/articles/markdown-basics/#multiple-lines).
* **Describe the behavior you observed after following the steps** and point out what exactly is the problem with that behavior.
* **Explain which behavior you expected to see instead and why.**
* **Include screenshots and animated GIFs** which show you following the described steps and clearly demonstrate the problem. You can use [this tool](https://www.cockos.com/licecap/) to record GIFs on macOS and Windows, and [this tool](https://github.com/colinkeenan/silentcast) or [this tool](https://github.com/GNOME/byzanz) on Linux.
* **If the problem wasn't triggered by a specific action**, describe what you were doing before the problem happened and share more information using the guidelines below.

Provide more context by answering these questions:

* **Did the problem start happening recently** (e.g. after updating to a new version of Sqlite3Utils) or was this always a problem?
* If the problem started happening recently, **can you reproduce the problem in an older version of Sqlite3Utils?** What's the most recent version in which the problem doesn't happen? You can download older versions of Sqlite3Utils from [the releases page](https://github.com/AEduardo-dev/Sqlite3Utils/releases).
* **Can you reliably reproduce the issue?** If not, provide details about how often the problem happens and under which conditions it normally happens.

Include details about your configuration and environment:

<!-- * **Which version of Sqlite3Utils are you using?** You can get the exact version by running Sqlite3Utils -v in your terminal. -->
* **What's the name and version of the OS you're using**?
* **Which c++ compiler are you using**?

### Suggesting Enhancements

This section guides you through submitting an enhancement suggestion for Sqlite3Utils, including completely new features and minor improvements to existing functionality. Following these guidelines helps maintainers and the community understand your suggestion :pencil: and find related suggestions :mag_right:.

Before creating enhancement suggestions, please check [this list](#before-submitting-an-enhancement-suggestion) as you might find out that you don't need to create one. When you are creating an enhancement suggestion, please [include as many details as possible](#how-do-i-submit-a-good-enhancement-suggestion). Fill in [the template](https://github.com/AEduardo-dev/Sqlite3Utils/blob/main/.github/ISSUE_TEMPLATE/feature_request.md), including the steps that you imagine you would take if the feature you're requesting existed.

#### Before Submitting An Enhancement Suggestion

* **Check if you're using the latest version of Sqlite3Utils.**
* **Perform a [search](https://github.com/AEduardo-dev/Sqlite3Utils/issues?q=is%3Aissue+label%3Aenhancement)** to see if the enhancement has already been suggested. If it has, add a comment to the existing issue instead of opening a new one.

#### How Do I Submit A (Good) Enhancement Suggestion?

Enhancement suggestions are tracked as [GitHub issues](https://guides.github.com/features/issues/). Create an issue on the repository and provide the following information:

* **Use a clear and descriptive title** for the issue to identify the suggestion.
* **Provide a step-by-step description of the suggested enhancement** in as many details as possible.
* **Provide specific examples to demonstrate the steps**. Include copy/pasteable snippets which you use in those examples, as [Markdown code blocks](https://help.github.com/articles/markdown-basics/#multiple-lines).
* **Describe the current behavior** and **explain which behavior you expected to see instead** and why.
* **Include screenshots and animated GIFs** which help you demonstrate the steps or point out the part of Sqlite3Utils which the suggestion is related to. You can use [this tool](https://www.cockos.com/licecap/) to record GIFs on macOS and Windows, and [this tool](https://github.com/colinkeenan/silentcast) or [this tool](https://github.com/GNOME/byzanz) on Linux.
* **Explain why this enhancement would be useful** to most Sqlite3Utils users.
<!-- * **Specify which version of Sqlite3Utils you're using.** You can get the exact version by running `Sqlite3Utils -v` in your terminal. -->
* **Specify the name and version of the OS you're using.**

<!-- ### Your First Code Contribution -->

<!-- ### Pull Requests

The process described here has several goals:

- Maintain Sqlite3Utils's quality
- Fix problems that are important to users
- Engage the community in working toward the best possible Sqlite3Utils
- Enable a sustainable system for Sqlite3Utils's maintainers to review contributions

Please follow these steps to have your contribution considered by the maintainers:

1. Follow all instructions in [the template](PULL_REQUEST_TEMPLATE.md)
2. Follow the [styleguides](#styleguides)
3. After you submit your pull request, verify that all [status checks](https://help.github.com/articles/about-status-checks/) are passing <details><summary>What if the status checks are failing?</summary>If a status check is failing, and you believe that the failure is unrelated to your change, please leave a comment on the pull request explaining why you believe the failure is unrelated. A maintainer will re-run the status check for you. If we conclude that the failure was a false positive, then we will open an issue to track that problem with our status check suite.</details>

While the prerequisites above must be satisfied prior to having your pull request reviewed, the reviewer(s) may ask you to complete additional design work, tests, or other changes before your pull request can be ultimately accepted. -->


## Additional Notes

### Issue and Pull Request Labels

This section lists the labels we use to help us track and manage issues and pull requests.

[GitHub search](https://help.github.com/articles/searching-issues/) makes it easy to use labels for finding groups of issues or pull requests you're interested in. For example, you might be interested in [open issues across Sqlite3Utils which are labeled as bugs, but still need to be reliably reproduced](https://github.com/AEduardo-dev/Sqlite3Utils/issues?q=is%3Aopen+label%3A%22needs+reproduction%22+label%3Abug) or perhaps [open pull requests which haven't been reviewed yet](https://github.com/AEduardo-dev/Sqlite3Utils/pulls?q=is%3Aopen+is%3Apr). To help you find issues and pull requests, each label is listed with search links for finding open items with that label in Sqlite3Utils. We encourage you to read about [other search filters](https://help.github.com/articles/searching-issues/) which will help you write more focused queries.

The labels are loosely grouped by their purpose, but it's not required that every issue have a label from every group or that an issue can't have more than one label from the same group.

Please open an issue if you have suggestions for new labels.
