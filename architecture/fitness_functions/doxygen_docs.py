#!/usr/bin/env python3
"""
Doxygen Documentation Parser

Library for parsing Doxygen comments from C header files.
Used by FF-001 to generate API documentation.

Usage:
    from doxygen_docs import parse_header_file, generate_api_docs_html
"""

import re
import json
import html
from pathlib import Path
from typing import List, Dict, Optional, Tuple


def parse_doxygen_comment(lines: List[str], start_idx: int) -> Tuple[Optional[Dict], int]:
    """
    Parse a Doxygen comment block starting at the given index.

    Returns: (parsed_doc_dict, end_index) or (None, start_idx) if not a doc comment
    """
    if start_idx >= len(lines):
        return None, start_idx

    line = lines[start_idx].strip()

    # Check if this is a Doxygen comment (/** or /*!)
    if not (line.startswith('/**') or line.startswith('/*!')):
        return None, start_idx

    doc = {
        'brief': '',
        'description': '',
        'params': [],
        'returns': '',
        'note': '',
        'raw_lines': []
    }

    idx = start_idx
    in_comment = True

    while idx < len(lines) and in_comment:
        line = lines[idx].strip()
        doc['raw_lines'].append(line)

        # End of comment
        if '*/' in line:
            in_comment = False
            idx += 1
            break

        # Remove leading * and whitespace
        line = re.sub(r'^\s*\*\s?', '', line)
        line = re.sub(r'^/\*\*\s*', '', line)

        # Parse @brief
        if line.startswith('@brief'):
            doc['brief'] = line[6:].strip()
        # Parse @param
        elif line.startswith('@param'):
            match = re.match(r'@param\s+(\w+)\s+(.+)', line)
            if match:
                doc['params'].append({
                    'name': match.group(1),
                    'desc': match.group(2)
                })
        # Parse @return or @returns
        elif line.startswith(('@return', '@returns')):
            doc['returns'] = re.sub(r'@returns?\s+', '', line)
        # Parse @note
        elif line.startswith('@note'):
            doc['note'] = line[5:].strip()
        # Continuation lines (description text after @brief)
        elif line and not line.startswith('*'):
            if doc['note']:
                doc['note'] += '\n' + line
            elif doc['returns']:
                doc['returns'] += '\n' + line
            elif doc['brief']:
                # Lines after @brief become description
                if doc['description']:
                    doc['description'] += '\n' + line
                else:
                    doc['description'] = line

        idx += 1

    return doc, idx


def parse_function_signature(line: str) -> Optional[Dict]:
    """
    Parse a function signature line.

    Returns: dict with 'return_type', 'name', 'params' or None
    """
    # Match: return_type function_name( params );
    match = re.match(r'^\s*(\w+(?:\s*\*)?)\s+(\w+)\s*\((.*?)\)\s*;', line)
    if not match:
        return None

    return {
        'return_type': match.group(1).strip(),
        'name': match.group(2),
        'params_raw': match.group(3).strip()
    }


def parse_header_file(header_path: Path) -> List[Dict]:
    """
    Parse the header file and extract all documented functions.

    Returns: List of function documentation dicts
    """
    with open(header_path, 'r', encoding='utf-8') as f:
        lines = f.readlines()

    functions = []
    idx = 0

    while idx < len(lines):
        # Look for doxygen comment
        doc, idx = parse_doxygen_comment(lines, idx)

        if doc:
            # Next non-empty line should be the function signature
            while idx < len(lines):
                line = lines[idx].strip()
                if line and not line.startswith('//'):
                    func = parse_function_signature(line)
                    if func:
                        func['doc'] = doc
                        functions.append(func)
                    break
                idx += 1

        idx += 1

    return functions


def generate_api_docs_html(functions: List[Dict], project_root: Path) -> None:
    """
    Generate a beautiful dark-mode HTML API reference.

    Args:
        functions: List of parsed function dictionaries
        project_root: Path to project root directory
    """
    html = """<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Archimedes API Reference</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }
        body {
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', 'Roboto', 'Inter', sans-serif;
            background: #0d1117;
            color: #c9d1d9;
            padding: 40px 20px;
            line-height: 1.6;
        }
        .container {
            max-width: 1200px;
            margin: 0 auto;
        }
        header {
            margin-bottom: 48px;
            border-bottom: 1px solid #30363d;
            padding-bottom: 24px;
        }
        h1 {
            font-size: 2.5em;
            font-weight: 600;
            color: #f0f6fc;
            margin-bottom: 8px;
            letter-spacing: -0.03em;
        }
        .subtitle {
            font-size: 1.1em;
            color: #8b949e;
            font-weight: 400;
        }
        .function {
            background: #161b22;
            border: 1px solid #30363d;
            border-radius: 6px;
            padding: 24px;
            margin-bottom: 24px;
            transition: border-color 0.2s;
        }
        .function:hover {
            border-color: #58a6ff;
        }
        .function-signature {
            font-family: 'SF Mono', 'Monaco', 'Consolas', monospace;
            font-size: 1.1em;
            color: #79c0ff;
            margin-bottom: 16px;
            padding: 16px;
            background: #0d1117;
            border-radius: 6px;
            border: 1px solid #30363d;
            overflow-x: auto;
        }
        .return-type {
            color: #ff7b72;
        }
        .function-name {
            color: #d2a8ff;
            font-weight: 600;
        }
        .param {
            color: #ffa657;
        }
        .brief {
            font-size: 1.05em;
            color: #c9d1d9;
            margin-bottom: 16px;
            line-height: 1.6;
        }
        .section-title {
            font-size: 0.9em;
            font-weight: 600;
            color: #8b949e;
            text-transform: uppercase;
            letter-spacing: 0.05em;
            margin-top: 16px;
            margin-bottom: 8px;
        }
        .param-list {
            list-style: none;
        }
        .param-item {
            padding: 8px 0;
            border-bottom: 1px solid #21262d;
        }
        .param-item:last-child {
            border-bottom: none;
        }
        .param-name {
            font-family: 'SF Mono', 'Monaco', 'Consolas', monospace;
            color: #ffa657;
            font-weight: 600;
        }
        .param-desc {
            color: #8b949e;
            margin-left: 8px;
        }
        .returns {
            color: #8b949e;
            padding: 12px;
            background: #0d1117;
            border-radius: 6px;
            border-left: 3px solid #58a6ff;
        }
        .note {
            color: #8b949e;
            padding: 12px;
            background: #0d1117;
            border-radius: 6px;
            border-left: 3px solid #d29922;
            font-style: italic;
        }
        .stats {
            margin-top: 48px;
            padding: 24px;
            background: #161b22;
            border: 1px solid #30363d;
            border-radius: 6px;
            text-align: center;
        }
        .stats-title {
            font-size: 1.1em;
            color: #f0f6fc;
            font-weight: 600;
            margin-bottom: 8px;
        }
        .stats-value {
            font-size: 2em;
            color: #58a6ff;
            font-weight: 700;
        }
        .footer {
            margin-top: 16px;
            color: #6e7681;
            font-size: 0.9em;
        }
    </style>
</head>
<body>
    <div class="container">
        <header>
            <h1>📚 Archimedes API Reference</h1>
            <div class="subtitle">Complete function documentation</div>
        </header>
"""

    for func in functions:
        doc = func['doc']

        # Build function signature with syntax highlighting
        signature = f'<span class="return-type">{func["return_type"]}</span> '
        signature += f'<span class="function-name">{func["name"]}</span>('

        if func['params_raw']:
            signature += f'<span class="param">{func["params_raw"]}</span>'

        signature += ')'

        html += f"""
        <div class="function">
            <div class="function-signature">{signature}</div>
"""

        if doc['brief']:
            html += f"""            <div class="brief">{doc['brief']}</div>
"""

        if doc['params']:
            html += """            <div class="section-title">Parameters</div>
            <ul class="param-list">
"""
            for param in doc['params']:
                html += f"""                <li class="param-item">
                    <span class="param-name">{param['name']}</span>
                    <span class="param-desc">{param['desc']}</span>
                </li>
"""
            html += """            </ul>
"""

        if doc['returns']:
            html += f"""            <div class="section-title">Returns</div>
            <div class="returns">{doc['returns']}</div>
"""

        if doc['note']:
            html += f"""            <div class="section-title">Note</div>
            <div class="note">{doc['note']}</div>
"""

        html += """        </div>
"""

    html += f"""
        <div class="stats">
            <div class="stats-title">Documented Functions</div>
            <div class="stats-value">{len(functions)}</div>
            <div class="footer">Generated from Doxygen comments • Archimedes Architecture</div>
        </div>
    </div>
</body>
</html>
"""

    # Write HTML file
    output_dir = project_root / "architecture" / "documents"
    output_dir.mkdir(parents=True, exist_ok=True)
    output_path = output_dir / "api_reference.html"

    with open(output_path, 'w', encoding='utf-8') as f:
        f.write(html)

    print(f"📚 API documentation generated: {output_path}")
    print(f"   Found {len(functions)} documented functions")
    print(f"   Open in browser to view the API reference!")
    print()


def generate_unified_api_html(categorized_funcs: Dict[str, List], documented_funcs: List[Dict], project_root: Path) -> None:
    """
    Generate a unified API reference with function taxonomy + expandable Doxygen docs.

    Args:
        categorized_funcs: Dict of {"draw": [(name, line), ...], "blit": [...], ...}
        documented_funcs: List of parsed function dicts with Doxygen comments
        project_root: Path to project root directory
    """
    # Build lookup map: function_name -> documentation
    docs_map = {func['name']: func['doc'] for func in documented_funcs}

    # Category info
    category_info = {
        "draw": ("🎨", "Drawing Primitives", "Functions that create pixels procedurally"),
        "blit": ("📋", "Blitting Operations", "Functions that copy pre-rendered pixels"),
        "used": ("🔧", "Object Functions", "Functions following object-first pattern"),
        "lifecycle": ("♻️", "Lifecycle Functions", "Init, quit, load, save, etc."),
        "utility": ("🛠️", "Utility Functions", "Helper functions and special cases"),
        "unknown": ("❓", "Unclassified", "Functions that need categorization"),
        "violation": ("🚨", "Violations", "Functions breaking naming conventions")
    }

    html = """<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Archimedes API Reference</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }
        body {
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', 'Roboto', 'Inter', sans-serif;
            background: #0d1117;
            color: #c9d1d9;
            padding: 40px 20px;
            line-height: 1.6;
        }
        .container {
            max-width: 1400px;
            margin: 0 auto;
        }
        header {
            margin-bottom: 48px;
            border-bottom: 1px solid #30363d;
            padding-bottom: 24px;
        }
        h1 {
            font-size: 2.5em;
            font-weight: 600;
            color: #f0f6fc;
            margin-bottom: 8px;
            letter-spacing: -0.03em;
        }
        .subtitle {
            font-size: 1.1em;
            color: #8b949e;
            font-weight: 400;
        }
        .category {
            background: #161b22;
            border: 1px solid #30363d;
            border-radius: 6px;
            padding: 24px;
            margin-bottom: 24px;
            transition: border-color 0.2s;
        }
        .category:hover {
            border-color: #58a6ff;
        }
        .category-header {
            display: flex;
            align-items: center;
            gap: 12px;
            margin-bottom: 16px;
            padding-bottom: 12px;
            border-bottom: 1px solid #30363d;
        }
        .category-title {
            font-size: 1.4em;
            font-weight: 600;
            color: #f0f6fc;
        }
        .category-count {
            background: #30363d;
            color: #8b949e;
            padding: 4px 12px;
            border-radius: 12px;
            font-size: 0.85em;
            font-weight: 500;
        }
        .category-description {
            color: #8b949e;
            font-size: 0.95em;
            margin-bottom: 16px;
        }
        .function-list {
            display: flex;
            flex-direction: column;
            gap: 4px;
        }
        .function-item {
            background: #0d1117;
            border: 1px solid #30363d;
            border-radius: 6px;
            overflow: hidden;
        }
        .function-item:hover {
            border-color: #58a6ff;
        }
        .function-header {
            padding: 12px 16px;
            cursor: pointer;
            display: flex;
            justify-content: space-between;
            align-items: center;
            user-select: none;
        }
        .function-header:hover {
            background: #161b22;
        }
        .function-name {
            font-family: 'SF Mono', 'Monaco', 'Consolas', monospace;
            font-size: 0.9em;
            color: #c9d1d9;
        }
        .line-number {
            color: #6e7681;
            font-size: 0.85em;
            margin-left: 8px;
        }
        .expand-icon {
            color: #58a6ff;
            font-size: 0.8em;
            transform: rotate(0deg);
            transition: transform 0.15s;
        }
        .expand-icon.expanded {
            transform: rotate(180deg);
        }
        .function-docs {
            display: none;
            border-top: 1px solid #30363d;
        }
        .function-docs.expanded {
            display: block;
        }
        .docs-content {
            padding: 16px;
        }
        .brief {
            font-size: 1.05em;
            color: #f0f6fc;
            margin-bottom: 12px;
            line-height: 1.5;
            font-weight: 600;
        }
        .description {
            font-size: 0.95em;
            color: #8b949e;
            margin-bottom: 16px;
            line-height: 1.6;
            white-space: pre-line;
        }
        .section-title {
            font-size: 0.85em;
            font-weight: 600;
            color: #8b949e;
            text-transform: uppercase;
            letter-spacing: 0.05em;
            margin-top: 12px;
            margin-bottom: 6px;
        }
        .param-list {
            list-style: none;
        }
        .param-item {
            padding: 6px 0;
            font-size: 0.9em;
        }
        .param-name {
            font-family: 'SF Mono', 'Monaco', 'Consolas', monospace;
            color: #ffa657;
            font-weight: 600;
        }
        .param-desc {
            color: #8b949e;
            margin-left: 8px;
        }
        .returns, .note {
            font-size: 0.9em;
            color: #8b949e;
            padding: 8px 12px;
            background: #0d1117;
            border-radius: 4px;
            border-left: 3px solid #58a6ff;
        }
        .note {
            border-left-color: #d29922;
            font-style: italic;
        }
        .no-docs {
            color: #6e7681;
            font-style: italic;
            font-size: 0.9em;
            padding: 12px;
        }
        .stats {
            margin-top: 48px;
            padding: 24px;
            background: #161b22;
            border: 1px solid #30363d;
            border-radius: 6px;
            text-align: center;
        }
        .stats-grid {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
            gap: 24px;
            margin-bottom: 16px;
        }
        .stat-item {
            padding: 16px;
        }
        .stat-label {
            font-size: 0.9em;
            color: #8b949e;
            margin-bottom: 8px;
        }
        .stat-value {
            font-size: 2em;
            color: #58a6ff;
            font-weight: 700;
        }
        .footer {
            margin-top: 16px;
            color: #6e7681;
            font-size: 0.9em;
        }
        /* Category-specific accent colors */
        .category[data-category="draw"]:hover { border-color: #f85149; }
        .category[data-category="blit"]:hover { border-color: #58a6ff; }
        .category[data-category="used"]:hover { border-color: #3fb950; }
        .category[data-category="lifecycle"]:hover { border-color: #d29922; }
        .category[data-category="utility"]:hover { border-color: #a371f7; }
        .category[data-category="unknown"]:hover { border-color: #8b949e; }
        .category[data-category="violation"]:hover { border-color: #f85149; }
    </style>
</head>
<body>
    <div class="container">
        <header>
            <h1>📚 Archimedes API Reference</h1>
            <div class="subtitle">Complete function documentation with naming taxonomy</div>
        </header>
"""

    total_functions = 0
    documented_count = 0

    # Process categories in order
    for category in ["draw", "blit", "lifecycle", "utility", "unknown", "violation"]:
        funcs = categorized_funcs.get(category, [])
        if not funcs:
            continue

        emoji, title, description = category_info[category]
        total_functions += len(funcs)

        html += f"""
        <div class="category" data-category="{category}">
            <div class="category-header">
                <span>{emoji}</span>
                <span class="category-title">{title}</span>
                <span class="category-count">{len(funcs)}</span>
            </div>
            <div class="category-description">{description}</div>
            <div class="function-list">
"""

        for func_name, line_num in sorted(funcs):
            # Check if this function has documentation
            doc = docs_map.get(func_name)
            has_docs = doc is not None

            if has_docs:
                documented_count += 1

            # Store doc data as JSON in data attribute for lazy rendering
            from html import escape as html_escape
            doc_json = json.dumps(doc) if has_docs else ""
            doc_attr = html_escape(doc_json, quote=True) if doc_json else ""

            html += f"""                <div class="function-item" data-func="{func_name}" data-doc='{doc_attr}'>
                    <div class="function-header" onclick="toggleDocs(this)">
                        <div>
                            <span class="function-name">{func_name}()</span>
                            <span class="line-number">:{line_num}</span>
                        </div>
                        <span class="expand-icon">▼</span>
                    </div>
                    <div class="function-docs"></div>
                </div>
"""

        html += """            </div>
        </div>
"""

    # Now handle object functions (used:ObjectName categories)
    object_categories = {k: v for k, v in categorized_funcs.items() if k.startswith("used:")}
    if object_categories:
        # Group all object functions under one main category
        total_object_funcs = sum(len(funcs) for funcs in object_categories.values())
        total_functions += total_object_funcs

        html += f"""
        <div class="category" data-category="used">
            <div class="category-header">
                <span>🔧</span>
                <span class="category-title">Object Functions</span>
                <span class="category-count">{total_object_funcs}</span>
            </div>
            <div class="category-description">Functions following object-first pattern</div>
"""

        # Render each object type as a subsection
        for obj_category in sorted(object_categories.keys()):
            obj_name = obj_category.split(":", 1)[1]  # Extract "Timer" from "used:Timer"
            funcs = object_categories[obj_category]

            html += f"""
            <div style="margin-top: 16px; margin-bottom: 16px;">
                <h3 style="color: #58a6ff; font-size: 1.1em; margin-bottom: 8px; padding-left: 8px; border-left: 3px solid #58a6ff;">
                    {obj_name} Functions <span style="color: #6e7681; font-size: 0.85em; font-weight: normal;">({len(funcs)})</span>
                </h3>
                <div class="function-list">
"""

            for func_name, line_num in sorted(funcs):
                # Check if this function has documentation
                doc = docs_map.get(func_name)
                has_docs = doc is not None

                if has_docs:
                    documented_count += 1

                # Store doc data as JSON in data attribute for lazy rendering
                from html import escape as html_escape
                doc_json = json.dumps(doc) if has_docs else ""
                doc_attr = html_escape(doc_json, quote=True) if doc_json else ""

                html += f"""                <div class="function-item" data-func="{func_name}" data-doc='{doc_attr}'>
                    <div class="function-header" onclick="toggleDocs(this)">
                        <div>
                            <span class="function-name">{func_name}()</span>
                            <span class="line-number">:{line_num}</span>
                        </div>
                        <span class="expand-icon">▼</span>
                    </div>
                    <div class="function-docs"></div>
                </div>
"""

            html += """                </div>
            </div>
"""

        html += """        </div>
"""

    html += f"""
        <div class="stats">
            <div class="stats-grid">
                <div class="stat-item">
                    <div class="stat-label">Total Functions</div>
                    <div class="stat-value">{total_functions}</div>
                </div>
                <div class="stat-item">
                    <div class="stat-label">Documented</div>
                    <div class="stat-value">{documented_count}</div>
                </div>
                <div class="stat-item">
                    <div class="stat-label">Coverage</div>
                    <div class="stat-value">{round(100 * documented_count / total_functions) if total_functions else 0}%</div>
                </div>
            </div>
            <div class="footer">Generated by FF-001 • Archimedes Architecture</div>
        </div>
    </div>

    <script>
        let currentlyOpen = null;

        function renderDocs(doc) {{
            if (!doc) {{
                return '<div class="docs-content"><div class="no-docs">No documentation available</div></div>';
            }}

            let html = '<div class="docs-content">';

            if (doc.brief) {{
                html += `<div class="brief">${{doc.brief}}</div>`;
            }}

            if (doc.description) {{
                html += `<div class="description">${{doc.description}}</div>`;
            }}

            if (doc.params && doc.params.length > 0) {{
                html += '<div class="section-title">Parameters</div><ul class="param-list">';
                for (const param of doc.params) {{
                    html += `<li class="param-item"><span class="param-name">${{param.name}}</span><span class="param-desc">${{param.desc}}</span></li>`;
                }}
                html += '</ul>';
            }}

            if (doc.returns) {{
                html += `<div class="section-title">Returns</div><div class="returns">${{doc.returns}}</div>`;
            }}

            if (doc.note) {{
                html += `<div class="section-title">Note</div><div class="note">${{doc.note}}</div>`;
            }}

            html += '</div>';
            return html;
        }}

        function toggleDocs(header) {{
            const docsDiv = header.nextElementSibling;
            const icon = header.querySelector('.expand-icon');
            const funcItem = header.parentElement;

            // Close currently open docs if it's a different one
            if (currentlyOpen && currentlyOpen !== docsDiv) {{
                currentlyOpen.classList.remove('expanded');
                currentlyOpen.innerHTML = '';
                const prevIcon = currentlyOpen.previousElementSibling.querySelector('.expand-icon');
                if (prevIcon) prevIcon.classList.remove('expanded');
            }}

            // Toggle current
            const isExpanding = !docsDiv.classList.contains('expanded');

            if (isExpanding) {{
                // Lazy load docs on first expand
                if (!docsDiv.innerHTML) {{
                    const docData = funcItem.dataset.doc;
                    const doc = docData ? JSON.parse(docData) : null;
                    docsDiv.innerHTML = renderDocs(doc);
                }}
                docsDiv.classList.add('expanded');
                icon.classList.add('expanded');
                currentlyOpen = docsDiv;
            }} else {{
                docsDiv.classList.remove('expanded');
                icon.classList.remove('expanded');
                currentlyOpen = null;
            }}
        }}
    </script>
</body>
</html>
"""

    # Write unified HTML file
    output_dir = project_root / "architecture" / "documents"
    output_dir.mkdir(parents=True, exist_ok=True)
    output_path = output_dir / "api_reference.html"

    with open(output_path, 'w', encoding='utf-8') as f:
        f.write(html)

    print(f"📚 API reference generated: {output_path}")
    print(f"   {total_functions} functions total, {documented_count} documented ({round(100 * documented_count / total_functions) if total_functions else 0}% coverage)")
    print(f"   Click any function to expand documentation!")
    print()


# This is a library module - import and use the functions above
