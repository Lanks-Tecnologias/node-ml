import { join } from 'path';
import { fileURLToPath } from 'url';
import { createRequire } from 'module';
// Re-export the native module
const require = createRequire(import.meta.url);
const __filename = fileURLToPath(import.meta.url);
const __dirname = join(__filename.slice(0, -8), '.');
let nativeBinding = null;
try {
  nativeBinding = require(join(__dirname, 'bin', 'ml.node'));
} catch (err) {
  throw new Error('Failed to load native binding: ' + err);
}
console.log(nativeBinding)

export default nativeBinding;
