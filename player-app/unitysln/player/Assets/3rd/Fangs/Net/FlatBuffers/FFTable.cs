
namespace FlatBuffers
{
	public class FFTable : Table
	{
		public new int bb_pos { get { return base.bb_pos; } set { base.bb_pos = value; } }
		public new ByteBuffer bb { get { return base.bb; } set { base.bb = value; } }
		
		public new int __offset (int vtableOffset)
		{
			return base.__offset (vtableOffset);
		}
		
		public new int __indirect (int offset)
		{
			return offset + bb.GetInt (offset);
		}
		
		public new string __string (int offset)
		{
			return base.__string (offset);
		}
		
		public new int __vector_len (int offset)
		{
			return base.__vector_len (offset);
		}
		
		public new int __vector (int offset)
		{
			return base.__vector (offset);
		}
		
		public FFTable __union (FFTable t, int offset)
		{
			return base.__union (t as Table, offset) as FFTable;
		}
		
		public new static bool __has_identifier (ByteBuffer bb, string ident)
		{
			return Table.__has_identifier (bb, ident);
		}

	}
}//namespace

