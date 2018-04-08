/********************************************************************
     Copyright (c) zyncho
     All rights reserved.

     author: Easlee Liu
*********************************************************************/

using UnityEngine;
using System.Collections.Generic;
using Fangs.Lib.Logger;

namespace VRXX.Manager
{
    public class Block
    {
        public string path = "";
        public string action = "";
        public Dictionary<string, string> param = new Dictionary<string, string>(0);
        public List<string> nexts = new List<string>(0);

        public void Execute()
        {
            this.LogDebug("Ready execute block {0}", action);
            param["path"] = path;
            VRXX.Action.ActionHub.Invoke(action, param, () =>
            {
                this.LogDebug("Execute block {0} finish", action);
                foreach(string next in nexts)
                {
                    Block block = BlockMgr.FindBlock(next);
                    block.Execute();
                }
            });
        }
    }

    public static class BlockMgr
    {
        private delegate void CallBack();

        private static Transform root_ { get; set; }
        private static Dictionary<string, Block> blocks = new Dictionary<string, Block>(0);

        public static void Preload()
        {
            root_ = new GameObject("__BlockMgr__").transform;
            GameObject.DontDestroyOnLoad(root_.gameObject);
            Log.Info("BlockMgr", "Preload SlateMgr finish");
        }

        public static Block NewBlock(string _guid)
        {
            if (blocks.ContainsKey(_guid))
                return null;
            Block block = new Block();
            blocks.Add(_guid, block);
            return block;
        }

        public static Block FindBlock(string _guid)
        {
            if (!blocks.ContainsKey(_guid))
                return null;
            return blocks[_guid];
        }

        public static string[] ListWithPath(string _path)
        {
            List<string> blockList = new List<string>(0);
            foreach(string uuid in blocks.Keys)
            {
                if (blocks[uuid].path.Equals(_path))
                    blockList.Add(uuid);
            }
            return blockList.ToArray();
        }

        public static void Clean()
        {
            blocks.Clear();
        }

    }
}