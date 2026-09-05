import os
from pathlib import Path

SOURCE_DIR = Path("docs")
TARGET_DIR = Path("phoenix")

HTML_HEADER = """<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<title>Phoenix Document</title>

<script>
window.MathJax = {
    tex: { inlineMath: [['$', '$'], ['\\\\(', '\\\\)']] },
    svg: { fontCache: 'global' }
};
</script>
<script src="https://cdn.jsdelivr.net/npm/mathjax@3/es5/tex-svg.js"></script>

<script>
const savedTheme = localStorage.getItem('theme') || 'dark';
document.documentElement.setAttribute('data-theme', savedTheme);
</script>

<style>
:root {
    --bg: #0d0d0d;
    --fg: #ffffff;
    --fg-secondary: #e6e6e6;
    --link: #d0e0ff;
}
[data-theme="light"] {
    --bg: #ffffff;
    --fg: #222222;
    --fg-secondary: #555555;
    --link: #0050a0;
}
body {
    background: var(--bg);
    color: var(--fg);
    font-family: Segoe UI, Roboto, Helvetica, Arial, sans-serif;
    margin: 40px auto;
    max-width: 900px;
    line-height: 1.6;
}
a { color: var(--link); }
p { color: var(--fg-secondary); }
.theme-toggle {
    position: fixed;
    top: 20px;
    right: 20px;
    background: var(--fg);
    color: var(--bg);
    padding: 8px 14px;
    border-radius: 6px;
    cursor: pointer;
    font-size: 14px;
    user-select: none;
}
.nav-block {
    display: flex;
    justify-content: space-between;
    margin: 20px 0;
    padding: 10px 0;
    border-top: 1px solid var(--fg-secondary);
    border-bottom: 1px solid var(--fg-secondary);
}
.nav-link {
    color: var(--link);
    font-size: 16px;
    text-decoration: none;
}
.nav-link:hover {
    text-decoration: underline;
}
.nav-disabled {
    opacity: 0.4;
    cursor: default;
}
.nav-spacer {
    flex: 1;
}
</style>
</head>

<body>
<div class="theme-toggle" onclick="toggleTheme()">Toggle Theme</div>

<script>
function toggleTheme() {
    const current = document.documentElement.getAttribute('data-theme');
    const next = current == 'dark' ? 'light' : 'dark';
    document.documentElement.setAttribute('data-theme', next);
    localStorage.setItem('theme', next);
}
</script>
"""

HTML_FOOTER = """</body>
</html>
"""

def rewrite_links(line):
    if "[" not in line or "](" not in line:
        return line

    out = ""
    parts = line.split("[")
    out += parts[0]

    for p in parts[1:]:
        if "](" in p:
            text, rest = p.split("](", 1)
            url, tail = rest.split(")", 1)

            # .md → .html
            if url.endswith(".md"):
                url = url[:-3] + ".html"

            # docs/ → phoenix/docs/
            if url.startswith("docs/"):
                url = "phoenix/" + url[5:]

            # epochX/... → phoenix/epochX/...
            elif url.startswith("epoch") or url.startswith("canonical"):
                url = "phoenix/" + url

            # ./epochX/... → phoenix/epochX/...
            elif url.startswith("./epoch") or url.startswith("./canonical"):
                url = "phoenix/" + url[2:]

            out += f'<a href="{url}">{text}</a>' + tail
        else:
            out += p

    return out

def nav_block(prev_file, next_file):
    if prev_file:
        prev_html = f'<a class="nav-link" href="{prev_file}">⟵ Previous</a>'
    else:
        prev_html = '<span class="nav-link nav-disabled">⟵ Previous</span>'

    if next_file:
        next_html = f'<a class="nav-link" href="{next_file}">Next ⟶</a>'
    else:
        next_html = '<span class="nav-link nav-disabled">Next ⟶</span>'

    return f"""
<div class="nav-block">
    <a class="nav-link" href="/index.html">⟵ Phoenix Index</a>
    <span class="nav-spacer"></span>
    {prev_html}
    <span class="nav-spacer"></span>
    {next_html}
</div>
"""

def convert_markdown_to_html(md_text, prev_file, next_file):
    lines = md_text.split("\n")
    out = []

    # Navigation at top
    out.append(nav_block(prev_file, next_file))

    for line in lines:
        if line.startswith("# "):
            out.append("<h1>" + line[2:] + "</h1>")
            continue
        if line.startswith("## "):
            out.append("<h2>" + line[3:] + "</h2>")
            continue
        if line.startswith("### "):
            out.append("<h3>" + line[4:] + "</h3>")
            continue
        if line.startswith("#### "):
            out.append("<h4>" + line[5:] + "</h4>")
            continue

        line = rewrite_links(line)
        out.append(line)

    # Navigation at bottom
    out.append(nav_block(prev_file, next_file))

    return HTML_HEADER + "<br>\n".join(out) + HTML_FOOTER

def process():
    if not SOURCE_DIR.exists():
        print("docs/ directory not found.")
        return

    TARGET_DIR.mkdir(exist_ok=True)

    for root, dirs, files in os.walk(SOURCE_DIR):
        rel = Path(root).relative_to(SOURCE_DIR)
        out_dir = TARGET_DIR / rel
        out_dir.mkdir(parents=True, exist_ok=True)

        md_files = sorted([f for f in files if f.endswith(".md")])

        for i, f in enumerate(md_files):
            src = Path(root) / f
            dst = out_dir / f.replace(".md", ".html")

            prev_file = md_files[i - 1].replace(".md", ".html") if i > 0 else None
            next_file = md_files[i + 1].replace(".md", ".html") if i < len(md_files) - 1 else None

            with open(src, "r", encoding="utf-8") as md:
                text = md.read()

            html = convert_markdown_to_html(text, prev_file, next_file)

            with open(dst, "w", encoding="utf-8") as out:
                out.write(html)

            print("Converted:", src, "→", dst)

    print("\nPhoenix HTML conversion complete.")

if __name__ == "__main__":
    process()

