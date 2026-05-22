import { readFile, writeFile } from "node:fs/promises";

const htmlPath = new URL("./architecture.html", import.meta.url);
const jsonPath = new URL("./architecture.json", import.meta.url);

const html = await readFile(htmlPath, "utf8");
const architecture = JSON.parse(await readFile(jsonPath, "utf8"));
const embeddedJson = JSON.stringify(architecture);

const architectureDataBlock = /<script type="application\/json" id="architecture-data">[\s\S]*?<\/script>/;

if (!architectureDataBlock.test(html)) {
  throw new Error("Unable to find architecture-data script block in architecture.html");
}

const nextHtml = html.replace(
  architectureDataBlock,
  '<script type="application/json" id="architecture-data">' + embeddedJson + "</script>"
);

await writeFile(htmlPath, nextHtml);
