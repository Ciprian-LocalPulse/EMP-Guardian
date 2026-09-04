# Security Policy

**Maintainer:** Ciprian Ștefan Pleșca

## Reporting a vulnerability

If you discover a security vulnerability in EMP-Guardian — in the firmware, the monitoring software, the API, or the CI/CD configuration — please report it privately rather than opening a public issue.

**Preferred method:** use GitHub's [private vulnerability reporting](https://github.com/Ciprian-LocalPulse/EMP-Guardian/security/advisories/new) feature on this repository (Security tab → Report a vulnerability). This creates a private advisory visible only to the maintainer until a fix is ready.

## Scope

This policy covers:
- Firmware code in `firmware/`
- Monitoring and API software in `software/`
- CI/CD configuration in `.github/`
- Dependency vulnerabilities flagged by Dependabot

It does **not** cover general questions about EMP physics or protection design — use the [Wiki](https://github.com/Ciprian-LocalPulse/EMP-Guardian/wiki) or a regular issue for those.

## What to expect

- Acknowledgement of your report as soon as reasonably possible.
- An assessment of severity and, if valid, a plan for a fix.
- Credit in the release notes, if you'd like it, once the fix is published.

## Out of scope

Reports related to the theoretical/design limitations already documented in `MANIFESTO.md` and `docs/threat_model.md` (e.g., "the detection thresholds aren't experimentally calibrated") are known limitations, not vulnerabilities — feel free to discuss those in a regular issue or pull request instead.
