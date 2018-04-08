local core = require "core"
local log = require "log"
local diagram = require "diagram"
local canvas = require "canvas"

local extend = "basic"
local group = "Camera"
local template = "SetRotation"

local image = "Camera/Camera.png"

local prop_x = "x"
local prop_y = "y"
local prop_z = "z"

local label_x = "x"
local label_y = "y"
local label_z = "z"

local txt_setrotation = {en_US="Set Rotation", zh_CN="设置旋转"}
local txt_usesnap= {en_US="Snapshot", zh_CN="使用快照"}

log.info("register "..extend.."."..group.."."..template.." action")
diagram.registerBlock(extend, group, template)
diagram.blockUiUseImage(group, template, image)

diagram.blockRegisterProperty(group, template, prop_x, 0.0) 
diagram.blockRegisterProperty(group, template, prop_y, 0.0)
diagram.blockRegisterProperty(group, template, prop_z, 0.0)

--
-- Block UI
--
diagram.blockUiAddText(group, template, core.translate(txt_setrotation))

--
-- Inspector UI
-- 
local invoker_ = ""

function onInspectorBuildFinish(_params)
    invoker_ = _params.invoker
    log.debug("onInspectorBuildFinish, invoke is " .. invoker_)
end

function onButtonClick()
    log.trace("onButtonClick")
    local params = {}
    canvas.invokeMessage("JSQueryCameraRotation", invoker_, params)
end

diagram.inspectorBindCallback(group, template, "onBuildFinish", onInspectorBuildFinish)

diagram.inspectorUiBindNumberSpin(group, template, prop_x, label_x, -999999999.0, 999999999.0)
diagram.inspectorUiBindNumberSpin(group, template, prop_y, label_y, -999999999.0, 999999999.0)
diagram.inspectorUiBindNumberSpin(group, template, prop_z, label_z, -999999999.0, 999999999.0)
diagram.inspectorUiBindButton(group, template, core.translate(txt_usesnap), onButtonClick)


--
-- Canvas
--
function handleCanvasMessage(_message, _invoker, _params)
    if (_invoker ~= invoker_ )
    then
        return
    end

    if(_message == "JSOnQueryCameraRotation")
    then
        local props = {}
        props[prop_x] = _params.x
        props[prop_y] = _params.y
        props[prop_z] = _params.z
        log.debug("Handle JSOnQueryCameraRotation, x:" .. props[prop_x] .. " y:" .. props[prop_y] .. " z:" .. props[prop_z])
        diagram.inspectorUiUpdateProperty(_invoker, props)
    end
end

canvas.bindHandler(handleCanvasMessage)

