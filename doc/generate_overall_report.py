#!/usr/bin/env python3
from __future__ import annotations
from pathlib import Path
from typing import Optional
from collections import defaultdict
from dataclasses import dataclass
import argparse
import html


@dataclass
class ReportInfo:
    """Information about a discovered report."""

    variant: str
    platform: str
    build_type: Optional[str]
    relative_path: str


def discover_reports(root: Path) -> dict[str, dict[str, dict[Optional[str], str]]]:
    """Discover all report index.html files and organize them by variant/platform/build_type."""
    results = defaultdict(lambda: defaultdict(dict))

    for report_file in root.rglob("reports/index.html"):
        if report_info := _parse_report_path(report_file, root):
            results[report_info.variant][report_info.platform][report_info.build_type] = report_info.relative_path

    return dict(results)


def _parse_report_path(report_file: Path, root: Path) -> Optional[ReportInfo]:
    """Parse a report file path to extract variant, platform, and build type information."""
    try:
        relative_path = report_file.relative_to(root)
        path_parts = relative_path.parts

        # Expect at least: variant/platform/reports/index.html (4 parts)
        if len(path_parts) < 4 or path_parts[-2] != "reports" or path_parts[-1] != "index.html":
            return None

        # Extract variant/platform/[build_type] from path
        structure_parts = path_parts[:-2]  # Remove 'reports/index.html'

        if len(structure_parts) == 2:
            variant, platform = structure_parts
            build_type = None
        elif len(structure_parts) == 3:
            variant, platform, build_type = structure_parts
        else:
            return None

        return ReportInfo(variant=variant, platform=platform, build_type=build_type, relative_path=relative_path.as_posix())
    except ValueError:
        return None


# CSS styles for the HTML report
_CSS_STYLES = """
body{font-family:system-ui,-apple-system,Segoe UI,Roboto,Arial,sans-serif;margin:2rem auto;background:#f7f8fa;color:#222;max-width:1200px}
h1{margin:0 0 1.5rem;color:#2c3e50;text-align:center}
.reports-table{width:100%;max-width:900px;margin:0 auto;border-collapse:collapse;background:#fff;border-radius:8px;overflow:hidden;box-shadow:0 2px 8px rgba(0,0,0,.1)}
.reports-table th{background:#34495e;color:#fff;padding:1rem;text-align:left;font-weight:600}
.reports-table td{padding:0.75rem 1rem;border-bottom:1px solid #ecf0f1}
.reports-table tr:hover{background:#f8f9fa}
.variant-name{font-weight:600;color:#2c3e50;min-width:120px}
.platform-name{font-family:monospace;background:#ecf0f1;padding:0.25rem 0.5rem;border-radius:4px;font-size:0.9em;min-width:140px;display:inline-block}
.build-type{color:#7f8c8d;font-style:italic;min-width:100px;display:inline-block}
.report-link{display:inline-block;background:#3498db;color:#fff;padding:0.5rem 1rem;border-radius:6px;text-decoration:none;font-size:0.9rem;transition:background 0.2s;min-width:100px;text-align:center}
.report-link:hover{background:#2980b9;text-decoration:none}
.no-reports{text-align:center;padding:3rem;color:#7f8c8d;font-style:italic}
""".strip()


def render_html(reports_index: dict[str, dict[str, dict[Optional[str], str]]], title: str = "All Reports") -> str:
    """Generate HTML content for the reports index page."""
    if not reports_index:
        return _render_empty_page(title)

    html_parts = [
        "<!doctype html>",
        "<meta charset='utf-8'>",
        f"<title>{html.escape(title)}</title>",
        "<meta name='viewport' content='width=device-width,initial-scale=1'>",
        f"<style>{_CSS_STYLES}</style>",
        f"<h1>{html.escape(title)}</h1>",
    ]

    # Create table
    html_parts.extend(
        [
            "<table class='reports-table'>",
            "<thead>",
            "<tr>",
            "<th>Variant</th>",
            "<th>Platform</th>",
            "<th>Build Type</th>",
            "<th>Report</th>",
            "</tr>",
            "</thead>",
            "<tbody>",
        ]
    )

    for variant_name in sorted(reports_index.keys(), key=str.lower):
        html_parts.extend(_render_variant_rows(variant_name, reports_index[variant_name]))

    html_parts.extend(
        [
            "</tbody>",
            "</table>",
        ]
    )

    return "\n".join(html_parts)


def _render_empty_page(title: str) -> str:
    """Render an empty page when no reports are found."""
    return f"""<!doctype html>
<meta charset='utf-8'>
<title>{html.escape(title)}</title>
<meta name='viewport' content='width=device-width,initial-scale=1'>
<style>{_CSS_STYLES}</style>
<h1>{html.escape(title)}</h1>
<div class='no-reports'>No reports found.</div>"""


def _render_variant_rows(variant_name: str, platforms: dict[str, dict[Optional[str], str]]) -> list[str]:
    """Render table rows for a variant."""
    rows = []
    first_row = True

    for platform_name in sorted(platforms.keys(), key=str.lower):
        for build_type, relative_path in sorted(platforms[platform_name].items(), key=lambda item: ("" if item[0] is None else item[0].lower())):
            # Display variant name only on first row
            variant_cell = f"<td class='variant-name'>{html.escape(variant_name)}</td>" if first_row else "<td></td>"

            # Format build type
            if build_type is None:
                build_display = ""  # Empty cell for default/no build type
            else:
                build_display = f"<span class='build-type'>{html.escape(build_type)}</span>"

            rows.append(
                "<tr>"
                + variant_cell
                + f"<td><span class='platform-name'>{html.escape(platform_name)}</span></td>"
                + f"<td>{build_display}</td>"
                + f"<td><a href='{html.escape(relative_path)}' target='_blank' rel='noopener' class='report-link'>Open Report</a></td>"
                + "</tr>"
            )
            first_row = False

    return rows


def main() -> None:
    """Main entry point for the script."""
    parser = argparse.ArgumentParser(description="Generate a grouped HTML index for report pages.")
    parser.add_argument("--build-dir", type=Path, required=True, help="Root build directory to scan")
    parser.add_argument("-o", "--output", type=Path, help="Output HTML path (default: build_dir/index.html)")
    parser.add_argument("--title", type=str, default="All Reports", help="Page title")
    args = parser.parse_args()

    build_directory = args.build_dir.resolve()
    if not build_directory.exists():
        raise FileNotFoundError(f"Build directory does not exist: {build_directory}")

    reports_index = discover_reports(build_directory)
    html_content = render_html(reports_index, title=args.title)

    output_path = (args.output or (build_directory / "index.html")).resolve()
    output_path.write_text(html_content, encoding="utf-8")

    total_reports = sum(len(platforms) for variant in reports_index.values() for platforms in variant.values())
    print(f"Generated report index at {output_path} with {total_reports} report entries.")


if __name__ == "__main__":
    main()
