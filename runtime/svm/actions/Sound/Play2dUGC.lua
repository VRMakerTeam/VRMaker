local core = require "core"
local log = require "log"
local diagram = require "diagram"

local extend = "basic"
local group = "Sound"
local template = "Play2dUGC"

local image = "Sound/Sound.png"

local prop_asset = "cache_asset"
local prop_track = "track"

local txt_play2dugc = {en_US="Play recording", zh_CN="播放2D录音"}

log.info("register "..extend.."."..group.."."..template.." action")
diagram.registerBlock(extend, group, template)
diagram.blockUiUseImage(group, template, image)

diagram.blockRegisterProperty(group, template, prop_asset, "")
diagram.blockRegisterProperty(group, template, prop_track, 0)

diagram.blockUiAddText(group, template, core.translate(txt_play2dugc))

diagram.inspectorUiBindRecorder(group, template, prop_asset)
