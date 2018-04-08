namespace FlatBuffers
{
	public class FFStruct : Struct
	{
		public new int bb_pos { get { return base.bb_pos; } set { base.bb_pos = value; } }
		public new ByteBuffer bb { get { return base.bb; } set { base.bb = value; } }
	}
}//namespace 