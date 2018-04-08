/********************************************************************
     Copyright (c) zyncho
     All rights reserved.

     author: Easlee Liu
*********************************************************************/

using System.Collections.Generic;
using Fangs.Lib.Logger;
using VRXX.Manager;

namespace VRXX.Action.Exit
{
    public static class Invoke
    {
        public static void Execute(Dictionary<string, string> _params, ActionDelegate _onFinish)
        {
            string command = "";
            if (!_params.TryGetValue("command", out command))
            {
                Log.Error("Invoke", "need params command");
                return;
            }

            _onFinish();

            string path = _params["path"];
            foreach (string uuid in BlockMgr.ListWithPath(path))
            {
                Block block = BlockMgr.FindBlock(uuid);
                if (!block.action.Equals("Entry:OnCommandInvoke"))
                    continue;
                if (!block.param["command"].Equals(command))
                    continue;
                block.Execute();
            }
        }
    }

    public static class SwitchSlate
    {
        public static void Execute(Dictionary<string, string> _params, ActionDelegate _onFinish)
        {
            string slateAlias = "";
            if (!_params.TryGetValue("slate", out slateAlias))
            {
                Log.Error("SwitchSlate", "need params slate");
                return;
            }
            UIMgr.ClearAllUi();
            _onFinish();

            string uuid = SlateMgr.AliasToUUID(slateAlias);
            Log.Trace("Invoke::SwitchSlate", "Switch slate, alias:{0} uuid:{1}", slateAlias, uuid);
            SlateMgr.SwithSlate(uuid);
        }
    }
}

    