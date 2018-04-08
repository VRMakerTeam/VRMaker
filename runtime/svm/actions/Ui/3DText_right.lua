local core = require "core"
local log = require "log"
local diagram = require "diagram"

local extend = "VR"
local group = "Ui"
local template = "New3DText_Right"

local image = "Ui/Ui.png"

local txt_3DText = {en_US="3DText_Right", zh_CN="3D文本(右对齐)"}

local prop_text = "text"
local prop_size = "size"
local prop_font_color = "font:color"
local prop_background_color = "background:color"
local prop_animation = "animation"

local label_text = {en_US="Text", zh_CN="文本"}
local label_font_color = {en_US="FontColor", zh_CN="字体颜色"}
local label_bcakground_color = {en_US="BackGroundColor", zh_CN="背景颜色"}
local label_size = {en_US="FontSize", zh_CN="字体大小"}
local label_animation = {en_US="Insert Animation", zh_CN="插入动画"}

local item_null = {en_US="none", zh_CN="无"}
local item_fadein = {en_US="FadeIn", zh_CN="淡入"}
local item_pullin = {en_US="PullIn", zh_CN="拉近"}



log.info("register "..extend.."."..group.."."..template.." action")
diagram.registerBlock(extend, group, template)
diagram.blockUiUseImage(group, template, image)

diagram.blockRegisterProperty(group, template, prop_text, "")
diagram.blockRegisterProperty(group, template, prop_size, 40)
diagram.blockRegisterProperty(group, template, prop_animation, "")

--
-- Block UI
--
diagram.blockUiAddText(group, template, core.translate(txt_3DText)) 

--
-- Inspector UI
-- 
diagram.inspectorUiBindTextEdit(group, template, prop_text, core.translate(label_text))
diagram.inspectorUiBindIntegerSpin(group, template, prop_size, core.translate(label_size), 0, 300)
diagram.inspectorUiBindColorPicker(group, template, prop_font_color , core.translate(label_font_color), 255, 255, 255, 255)
diagram.inspectorUiBindColorPicker(group, template, prop_background_color, core.translate(label_bcakground_color), 0, 0, 0, 125)
diagram.inspectorUiBindComboBox(group, template, prop_animation, core.translate(label_animation), {core.translate(item_null), core.translate(item_fadein), core.translate(item_pullin)})
