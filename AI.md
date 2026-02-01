# AI and LLM Usage Policy

This document describes the use of AI and Large Language Models (LLMs) in the Hinder library.

## Overview

The Hinder library project actively uses AI assistance (specifically Claude Code by Anthropic) as a collaborative tool throughout the project lifecycle. We believe AI can speed up development while maintaining high code quality through proper human oversight.

## Transparency

**We are transparent about AI usage:**

- AI contributions are clearly attributed in git commits using `Co-Authored-By: Claude <noreply@anthropic.com>`
- Claude is listed as a contributor in the project's `CONTRIBUTORS.md` file
- All AI-generated content undergoes human review before merging

## Scope of AI Assistance

### Architecture and Design

**AI Role: Co-designer**

- Discuss architectural approaches and trade-offs
- Identify technical challenges and propose solutions
- Research design patterns and best practices
- Challenge design decisions when warranted

**Human Role: Decision maker**

- Define requirements, goals, and constraints
- Make final architectural decisions
- Approve phase transitions
- Set project priorities

### Documentation

**AI Role: Primary author with human review**

- Draft comprehensive documentation from architectural decisions
- Structure information for clarity and accessibility
- Generate examples and use cases
- Maintain consistency across documents

**Human Role: Reviewer and editor**

- Review for accuracy and completeness
- Edit for project-specific context
- Approve before commit
- Ensure documentation reflects actual project state

### Code Generation (Future)

**AI Role: Implementation assistant**

- Generate initial implementations following project standards
- Apply modern C++20/Qt idioms consistently
- Write unit tests and integration tests
- Refactor code for clarity and maintainability

**Human Role: Code reviewer**

- Review all generated code before merging
- Verify correctness and safety
- Test functionality
- Ensure adherence to project philosophy (see CLAUDE.md)

### Research and Problem Solving

**AI Role: Research assistant**

- Investigate libraries and frameworks
- Explore KDE/Qt APIs and integration approaches
- Analyze error messages and debugging
- Suggest alternative solutions

**Human Role: Validator**

- Verify research findings
- Test proposed solutions
- Make technology choices
- Evaluate trade-offs in project context

## Human Oversight and Review

**All AI contributions require human review:**

1. **Documentation**: Review for accuracy, completeness, and project fit
2. **Code**: Review for correctness, safety, testing, and style compliance
3. **Architecture**: Validate decisions align with project goals and constraints
4. **Research**: Verify findings and test recommendations

**Nothing is merged without explicit human approval.**

## Attribution Standards

**Git commits with AI assistance use this format:**

```
Commit title summarizing changes

- Bullet points with details
- More details

Co-Authored-By: Claude <noreply@anthropic.com>
```

**No promotional links or advertising in commit messages.**

## Limitations and Responsibilities

### AI Limitations

- **No runtime verification**: AI cannot execute code or verify runtime behavior
- **Knowledge cutoff**: AI training data has a cutoff date
- **No system access**: AI cannot test on actual KDE Plasma systems
- **Pattern matching**: AI generates based on patterns, not true understanding

### Human Responsibilities

- **Final authority**: Humans make all final decisions
- **Testing**: Humans must test all functionality
- **Security**: Humans must review for security implications
- **Correctness**: Humans verify all technical claims and code correctness
- **Project fit**: Humans ensure AI contributions align with project vision

## Benefits of AI Assistance

**Why we use AI:**

- **Accelerated development**: Draft comprehensive documentation faster
- **Consistency**: Maintain consistent style and structure across documents
- **Best practices**: Apply modern C++20/Qt idioms systematically
- **Exploration**: Quickly research alternatives and trade-offs
- **Documentation**: Generate clear, well-structured documentation
- **Code quality**: Enforce safety and style standards proactively

**AI is a tool, not a replacement for human judgment, creativity, or responsibility.**

## Questions or Concerns

If you have questions about AI usage in this project, please:

1. Open an issue on GitHub: https://github.com/tonywalker1/plasma-workflows/issues
2. Review this document for clarification
3. Check CLAUDE.md for AI collaboration guidelines

## Version History

- **1.0.0** (2025-10-10) - Initial AI usage policy