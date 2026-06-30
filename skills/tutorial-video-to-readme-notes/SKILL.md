---
name: tutorial-video-to-readme-notes
description: Use when the user provides a tutorial video URL and asks to record, summarize, or add the lesson into README or project study notes while matching the existing documentation style and local code.
---

# Tutorial Video to README Notes

## Core Pattern

Turn a tutorial URL plus the current codebase into a README section that matches the user's existing study-note style.

## Workflow

1. Inspect local context first:
   - Read `main.cpp` or the active lesson code.
   - Search `README.md` for related sections and nearby headings.
   - Identify the best insertion point by concept order, not just file end.

2. Use the video URL carefully:
   - Browse or search the URL/title when the user provides it.
   - If the video page is inaccessible, state that in the working update and infer only from stable signals: URL, playlist index, local code, and known tutorial topic.
   - Do not invent transcript-specific claims.

3. Write the README section:
   - Match the existing Chinese note style: concept summary, numbered subsections, C++ code blocks, `[!IMPORTANT]` / `[!TIP]` / `[!WARNING]` callouts, and practical summaries.
   - Explain the code intent, not just syntax.
   - Keep examples aligned with the current `main.cpp`.
   - Prefer terms like `std::vector` followed by English term in parentheses when useful, consistent with the existing README.

4. Check code comments and examples:
   - If current code has misleading comments or a small correctness issue directly related to the lesson, fix it.
   - Do not rewrite unrelated lesson code.
   - Preserve unrelated user changes.

5. Verify:
   - Run the existing build command when C++ files changed.
   - If only README changed, still run a quick `rg` check for the new heading and key terms.
   - Report any video-access limitation and the exact README location changed.

## README Section Checklist

- Add a clear heading with Chinese title plus English term.
- Explain why this topic exists compared with the previous related topic.
- Include the relevant local code shape.
- Explain each important API or syntax from the code.
- Add a practical "when to use" summary.
- Avoid overclaiming details from inaccessible videos.

## Common Mistakes

- Appending notes at the end when a natural adjacent section already exists.
- Copying the code without explaining why it is written that way.
- Treating a video URL as a transcript when only the title/topic is known.
- Editing broad unrelated README sections while adding one lesson note.
