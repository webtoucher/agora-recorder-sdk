import { createRequire } from 'module'

const { AgoraRecorder } = createRequire(import.meta.url)('../build/Release/AgoraRecorder.node')
export default AgoraRecorder
