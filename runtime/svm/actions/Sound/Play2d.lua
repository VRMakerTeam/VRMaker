local core = require "core"
local log = require "log"
local diagram = require "diagram"

local extend = "basic"
local group = "Sound"
local template = "Play2d"

local image = "Sound/Sound.png"

local prop_asset = "asset"
local prop_track = "track"

local txt_play2d = {en_US="Play sound", zh_CN="播放2D音效"}

log.info("register "..extend.."."..group.."."..template.." action")
diagram.registerBlock(extend, group, template)
diagram.blockUiUseImage(group, template, image)

diagram.blockRegisterProperty(group, template, prop_asset, "")
diagram.blockRegisterProperty(group, template, prop_track, 0)

diagram.blockUiAddText(group, template, core.translate(txt_play2d))

diagram.inspectorUiBindPicker(group, template, prop_asset, "", {"sound/", "ugc/sound"})
