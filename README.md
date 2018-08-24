ICBit integration/staging tree
=====================================

[![Build Status](https://travis-ci.org/ICBit/ICBit.svg?branch=master)](https://travis-ci.org/ICBit/ICBit)

https://ICBitcore.org

What is ICBit?
----------------

ICBit is an experimental new digital currency that enables instant payments to
anyone, anywhere in the world. ICBit uses peer-to-peer technology to operate
with no central authority: managing transactions and issuing money are carried
out collectively by the network. ICBit Core is the name of open source
software which enables the use of this currency.

For more information, as well as an immediately useable, binary version of
the ICBit Core software, see https://ICBit.org/en/download, or read the
[original whitepaper](https://ICBitcore.org/ICBit.pdf).

License
-------

ICBit Core is released under the terms of the MIT license. See [COPYING](COPYING) for more
information or see https://opensource.org/licenses/MIT.

Development Process
-------------------

The `master` branch is regularly built and tested, but is not guaranteed to be
completely stable. [Tags](https://github.com/ICBit/ICBit/tags) are created
regularly to indicate new official, stable release versions of ICBit Core.

The contribution workflow is described in [CONTRIBUTING.md](CONTRIBUTING.md).

The developer [mailing list](https://lists.linuxfoundation.org/mailman/listinfo/ICBit-dev)
should be used to discuss complicated or controversial changes before working
on a patch set.

Developer IRC can be found on Freenode at #ICBit-core-dev.

Testing
-------

Testing and code review is the bottleneck for development; we get more pull
requests than we can review and test on short notice. Please be patient and help out by testing
other people's pull requests, and remember this is a security-critical project where any mistake might cost people
lots of money.

### Automated Testing

Developers are strongly encouraged to write [unit tests](/doc/unit-tests.md) for new code, and to
submit new unit tests for old code. Unit tests can be compiled and run
(assuming they weren't disabled in configure) with: `make check`

There are also [regression and integration tests](/qa) of the RPC interface, written
in Python, that are run automatically on the build server.
These tests can be run (if the [test dependencies](/qa) are installed) with: `qa/pull-tester/rpc-tests.py`

The Travis CI system makes sure that every pull request is built for Windows
and Linux, OS X, and that unit and sanity tests are automatically run.

### Manual Quality Assurance (QA) Testing

Changes should be tested by somebody other than the developer who wrote the
code. This is especially important for large or high-risk changes. It is useful
to add a test plan to the pull request description if testing the changes is
not straightforward.

Translations
------------

Changes to translations as well as new translations can be submitted to
[ICBit Core's Transifex page](https://www.transifex.com/projects/p/ICBit/).

Translations are periodically pulled from Transifex and merged into the git repository. See the
[translation process](doc/translation_process.md) for details on how this works.

**Important**: We do not accept translation changes as GitHub pull requests because the next
pull from Transifex would automatically overwrite them again.

Translators should also subscribe to the [mailing list](https://groups.google.com/forum/#!forum/ICBit-translators).
