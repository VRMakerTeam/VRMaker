local core = require "core"
local log = require "log"
local diagram = require "diagram"
local canvas = require "canvas"

local extend = "basic"
local group = "Camera"
local template = "Move"

local image = "Camera/Camera.png"

local prop_posx = "posx"
local prop_posy = "posy"
local prop_posz = "posz"
local prop_rotx = "rotx"
local prop_roty = "roty"
local prop_rotz = "rotz"
local prop_duration = "duration"

local label_posx = "posx"
local label_posy = "posy"
local label_posz = "posz"
local label_rotx = "rotx"
local label_roty = "roty"
local label_rotz = "rotz"
local label_duration = {en_US="Duration", zh_CN="时间"}

local txt_move = {en_US="Pan", zh_CN="移动"}

local txt_snapposition = {en_US="Snapshot Position", zh_CN="位置快照"}
local txt_snaprotation = {en_US="Snapshot Rotation", zh_CN="旋转快照"}

log.info("register "..extend.."."..group.."."..template.." action")
diagram.registerBlock(extend, group, template)
diagram.blockUiUseImage(group, template, image)

diagram.blockRegisterProperty(group, template, prop_posx, 0.0) 
diagram.blockRegisterProperty(group, template, prop_posy, 0.0)
diagram.blockRegisterProperty(group, template, prop_posz, 0.0)
diagram.blockRegisterProperty(group, template, prop_rotx, 0.0) 
diagram.blockRegisterProperty(group, template, prop_roty, 0.0)
diagram.blockRegisterProperty(group, template, prop_rotz, 0.0)
diagram.blockRegisterProperty(group, template, prop_duration, 0.0)

--
-- Block UI
--
diagram.blockUiAddText(group, template, core.translate(txt_move))

--
-- Inspector UI
-- 
local invoker_ = ""

function onInspectorBuildFinish(_params)
    invoker_ = _params.invoker
    log.debug("onInspectorBuildFinish, invoke is " .. invoker_)
end

function onSnapPositionButtonClick()
    log.trace("onSnapPositionButtonClick")
    local params = {}
    canvas.invokeMessage("JSQueryCameraPosition", invoker_, params)
end

function onSnapRotationButtonClick()
    log.trace("onSnapRotationButtonClick")
    local params = {}
    canvas.invokeMessage("JSQueryCameraRotation", invoker_, params)
end

diagram.inspectorBindCallback(group, template, "onBuildFinish", onInspectorBuildFinish)

diagram.inspectorUiBindNumberSpin(group, template, prop_posx, label_posx, -999999999.0, 999999999.0)
diagram.inspectorUiBindNumberSpin(group, template, prop_posy, label_posy, -999999999.0, 999999999.0)
diagram.inspectorUiBindNumberSpin(group, template, prop_posz, label_posz, -999999999.0, 999999999.0)
diagram.inspectorUiBindButton(group, template, core.translate(txt_snapposition), onSnapPositionButtonClick)
diagram.inspectorUiBindNumberSpin(group, template, prop_rotx, label_rotx, -360.0, 360.0)
diagram.inspectorUiBindNumberSpin(group, template, prop_roty, label_roty, -360.0, 360.0)
diagram.inspectorUiBindNumberSpin(group, template, prop_rotz, label_rotz, -360.0, 360.0)
diagram.inspectorUiBindButton(group, template, core.translate(txt_snaprotation), onSnapRotationButtonClick)
diagram.inspectorUiBindNumberSpin(group, template, prop_duration, core.translate(label_duration), 0, 999999999.0)


--
-- Canvas
--
function handleCanvasMessage(_message, _invoker, _params)
    if (_invoker ~= invoker_ )
    then
        return
    end

    if(_message == "JSOnQueryCameraPosition")
    then
        local props = {}
        props[prop_posx] = _params.x
        props[prop_posy] = _params.y
        props[prop_posz] = _params.z
        log.debug("Handle JSOnQueryCameraPosition, x:" .. props[prop_posx] .. " y:" .. props[prop_posy] .. " z:" .. props[prop_posz])
        diagram.inspectorUiUpdateProperty(_invoker, props)
    end

    if(_message == "JSOnQueryCameraRotation")
    then
        local props = {}
        props[prop_rotx] = _params.x
        props[prop_roty] = _params.y
        props[prop_rotz] = _params.z
        log.debug("Handle JSOnQueryCameraPosition, x:" .. props[prop_rotx] .. " y:" .. props[prop_roty] .. " z:" .. props[prop_rotz])
        diagram.inspectorUiUpdateProperty(_invoker, props)
    end

end

canvas.bindHandler(handleCanvasMessage)

