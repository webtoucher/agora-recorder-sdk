import { createRequire } from 'module'

const { AgoraRecorderSdk } = createRequire(import.meta.url)('../build/Release/AgoraRecorderSdk.node')
export default AgoraRecorderSdk
