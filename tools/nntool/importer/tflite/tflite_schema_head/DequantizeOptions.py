# automatically generated by the FlatBuffers compiler, do not modify

# namespace: tflite_schema_head

import flatbuffers

class DequantizeOptions(object):
    __slots__ = ['_tab']

    @classmethod
    def GetRootAsDequantizeOptions(cls, buf, offset):
        n = flatbuffers.encode.Get(flatbuffers.packer.uoffset, buf, offset)
        x = DequantizeOptions()
        x.Init(buf, n + offset)
        return x

    @classmethod
    def DequantizeOptionsBufferHasIdentifier(cls, buf, offset, size_prefixed=False):
        return flatbuffers.util.BufferHasIdentifier(buf, offset, b"\x54\x46\x4C\x33", size_prefixed=size_prefixed)

    # DequantizeOptions
    def Init(self, buf, pos):
        self._tab = flatbuffers.table.Table(buf, pos)

def DequantizeOptionsStart(builder): builder.StartObject(0)
def DequantizeOptionsEnd(builder): return builder.EndObject()
