# Claude Code Project Configuration — Zhiye Unity Platform (nlurh)

## Usage

Open this project in Claude Code to auto-load the project context from `CLAUDE.md`. Use the following to invoke the dedicated skill:

- `/nlu2-dll` — DLL External Extension Knowledge Base: development templates, Y Language syntax, platform APIs, all DLL interfaces, build & test workflows
- Ask questions directly: "How do I build a DLL?", "How do I use Y Language?", "What interfaces does ui.dll have?"

## Skills

| Skill | File | Purpose |
|-------|------|---------|
| nlu2-dll | skills/nlu2-dll.md | Zhiye Platform DLL External Extension Knowledge Base (hierarchical index) |

## Project Structure

- `CLAUDE.md` — Project context, auto-loaded by Claude Code
- `skills/` — Skill definition files
- `settings.json` — Permissions and hooks configuration
