; ModuleID = 'box-opt.bc'
source_filename = "box.ll"

%_Object_vtable = type { i32, i32, i8*, %Object* (%Object*)*, %String* (%Object*)*, %Object* (%Object*)* }
%String = type { %_String_vtable*, i8* }
%_String_vtable = type { i32, i32, i8*, %Object* (%Object*)*, %String* (%Object*)*, %Object* (%Object*)*, i32 (%String*)*, %String* (%String*, %String*)*, %String* (%String*, i32, i32)* }
%Object = type { %_Object_vtable* }
%_Int_vtable = type { i32, i32, i8*, %Object* (%Object*)*, %String* (%Object*)*, %Object* (%Object*)* }
%Int = type { %_Int_vtable*, i32 }
%_Bool_vtable = type { i32, i32, i8*, %Object* (%Object*)*, %String* (%Object*)*, %Object* (%Object*)* }
%Bool = type { %_Bool_vtable*, i1 }
%_IO_vtable = type { i32, i32, i8*, %Object* (%Object*)*, %String* (%Object*)*, %Object* (%Object*)*, %IO* (%IO*, %String*)*, %IO* (%IO*, i32)*, %String* (%IO*)*, i32 (%IO*)* }
%IO = type { %_IO_vtable* }
%_Main_vtable = type { i32, i32, i8*, %Object* (%Object*)*, %String* (%Object*)*, %Object* (%Object*)*, %IO* (%IO*, %String*)*, %IO* (%IO*, i32)*, %String* (%IO*)*, i32 (%IO*)*, %Object* (%Main*)* }
%Main = type { %_Main_vtable* }

@str.Object = internal constant [7 x i8] c"Object\00"
@_Object_vtable_prototype = local_unnamed_addr constant %_Object_vtable { i32 0, i32 ptrtoint (%Object* getelementptr (%Object, %Object* null, i32 1) to i32), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.Object, i32 0, i32 0), %Object* (%Object*)* @Object_abort, %String* (%Object*)* @Object_type_name, %Object* (%Object*)* @Object_copy }
@str.Int = internal constant [4 x i8] c"Int\00"
@_Int_vtable_prototype = local_unnamed_addr constant %_Int_vtable { i32 1, i32 ptrtoint (%Int* getelementptr (%Int, %Int* null, i32 1) to i32), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.Int, i32 0, i32 0), %Object* (%Object*)* @Object_abort, %String* (%Object*)* @Object_type_name, %Object* (%Object*)* @Object_copy }
@str.Bool = internal constant [5 x i8] c"Bool\00"
@_Bool_vtable_prototype = local_unnamed_addr constant %_Bool_vtable { i32 2, i32 ptrtoint (%Bool* getelementptr (%Bool, %Bool* null, i32 1) to i32), i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.Bool, i32 0, i32 0), %Object* (%Object*)* @Object_abort, %String* (%Object*)* @Object_type_name, %Object* (%Object*)* @Object_copy }
@str.String = internal constant [7 x i8] c"String\00"
@_String_vtable_prototype = constant %_String_vtable { i32 3, i32 ptrtoint (%String* getelementptr (%String, %String* null, i32 1) to i32), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.String, i32 0, i32 0), %Object* (%Object*)* @Object_abort, %String* (%Object*)* @Object_type_name, %Object* (%Object*)* @Object_copy, i32 (%String*)* @String_length, %String* (%String*, %String*)* @String_concat, %String* (%String*, i32, i32)* @String_substr }
@str.IO = internal constant [3 x i8] c"IO\00"
@_IO_vtable_prototype = local_unnamed_addr constant %_IO_vtable { i32 4, i32 ptrtoint (%IO* getelementptr (%IO, %IO* null, i32 1) to i32), i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.IO, i32 0, i32 0), %Object* (%Object*)* @Object_abort, %String* (%Object*)* @Object_type_name, %Object* (%Object*)* @Object_copy, %IO* (%IO*, %String*)* @IO_out_string, %IO* (%IO*, i32)* @IO_out_int, %String* (%IO*)* @IO_in_string, i32 (%IO*)* @IO_in_int }
@str.Main = internal constant [5 x i8] c"Main\00"
@_Main_vtable_prototype = constant %_Main_vtable { i32 5, i32 ptrtoint (%Main* getelementptr (%Main, %Main* null, i32 1) to i32), i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.Main, i32 0, i32 0), %Object* (%Object*)* @Object_abort, %String* (%Object*)* @Object_type_name, %Object* (%Object*)* @Object_copy, %IO* (%IO*, %String*)* @IO_out_string, %IO* (%IO*, i32)* @IO_out_int, %String* (%IO*)* @IO_in_string, i32 (%IO*)* @IO_in_int, %Object* (%Main*)* @Main_main }
@str.1 = internal constant [14 x i8] c"<basic class>\00"
@String.1 = local_unnamed_addr constant %String { %_String_vtable* @_String_vtable_prototype, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.1, i32 0, i32 0) }
@str.0 = internal constant [7 x i8] c"box.cl\00"
@String.0 = local_unnamed_addr constant %String { %_String_vtable* @_String_vtable_prototype, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.0, i32 0, i32 0) }

declare void @abort() local_unnamed_addr

; Function Attrs: inaccessiblememonly mustprogress nofree nounwind willreturn
declare noalias noundef i8* @malloc(i32 noundef) local_unnamed_addr #0

declare %Object* @Object_abort(%Object*)

declare %String* @Object_type_name(%Object*)

declare %Object* @Object_copy(%Object*)

declare %IO* @IO_out_string(%IO*, %String*)

declare %IO* @IO_out_int(%IO*, i32)

declare %String* @IO_in_string(%IO*)

declare i32 @IO_in_int(%IO*)

declare i32 @String_length(%String*)

declare %String* @String_concat(%String*, %String*)

declare %String* @String_substr(%String*, i32, i32)

declare %Int* @create_Int(i32) local_unnamed_addr

define i32 @main() local_unnamed_addr {
entry:
  %vtpm.23.i = tail call dereferenceable_or_null(8) i8* @malloc(i32 8)
  %vtpm.25.i = icmp eq i8* %vtpm.23.i, null
  br i1 %vtpm.25.i, label %abort.i, label %Main_new.exit

abort.i:                                          ; preds = %entry
  tail call void @abort()
  unreachable

Main_new.exit:                                    ; preds = %entry
  %vtpm.24.i = bitcast i8* %vtpm.23.i to %Main*
  %vtpm.26.i = getelementptr %Main, %Main* %vtpm.24.i, i64 0, i32 0
  store %_Main_vtable* @_Main_vtable_prototype, %_Main_vtable** %vtpm.26.i, align 8
  %vtpm.5.i = tail call %Int* @create_Int(i32 7)
  %vtpm.6.i = getelementptr %Int, %Int* %vtpm.5.i, i64 0, i32 0
  %vtpm.7.i = load %_Int_vtable*, %_Int_vtable** %vtpm.6.i, align 8
  %vtpm.8.i = getelementptr %_Int_vtable, %_Int_vtable* %vtpm.7.i, i64 0, i32 5
  %tmp.0.i = load %Object* (%Object*)*, %Object* (%Object*)** %vtpm.8.i, align 8
  %vtpm.9.i = bitcast %Int* %vtpm.5.i to %Object*
  %tmp.1.i = tail call %Object* %tmp.0.i(%Object* %vtpm.9.i)
  %vtpm.11.i = getelementptr %Object, %Object* %tmp.1.i, i64 1
  %0 = bitcast %Object* %vtpm.11.i to i32*
  %vtpm.12.i = load i32, i32* %0, align 4
  %vtpm.18.i = bitcast i8* %vtpm.23.i to %IO*
  %tmp.3.i = tail call %IO* @IO_out_int(%IO* nonnull %vtpm.18.i, i32 %vtpm.12.i)
  ret i32 0
}

define %Object* @Main_main(%Main* %self) {
  %vtpm.5 = tail call %Int* @create_Int(i32 7)
  %vtpm.6 = getelementptr %Int, %Int* %vtpm.5, i64 0, i32 0
  %vtpm.7 = load %_Int_vtable*, %_Int_vtable** %vtpm.6, align 8
  %vtpm.8 = getelementptr %_Int_vtable, %_Int_vtable* %vtpm.7, i64 0, i32 5
  %tmp.0 = load %Object* (%Object*)*, %Object* (%Object*)** %vtpm.8, align 8
  %vtpm.9 = bitcast %Int* %vtpm.5 to %Object*
  %tmp.1 = tail call %Object* %tmp.0(%Object* %vtpm.9)
  %vtpm.11 = getelementptr %Object, %Object* %tmp.1, i64 1
  %1 = bitcast %Object* %vtpm.11 to i32*
  %vtpm.12 = load i32, i32* %1, align 4
  %vtpm.15 = getelementptr %Main, %Main* %self, i64 0, i32 0
  %vtpm.16 = load %_Main_vtable*, %_Main_vtable** %vtpm.15, align 8
  %vtpm.17 = getelementptr %_Main_vtable, %_Main_vtable* %vtpm.16, i64 0, i32 7
  %tmp.2 = load %IO* (%IO*, i32)*, %IO* (%IO*, i32)** %vtpm.17, align 8
  %vtpm.18 = bitcast %Main* %self to %IO*
  %tmp.3 = tail call %IO* %tmp.2(%IO* %vtpm.18, i32 %vtpm.12)
  %vtpm.19 = bitcast %IO* %tmp.3 to %Object*
  ret %Object* %vtpm.19
}

define noalias %Main* @Main_new() local_unnamed_addr {
entry:
  %vtpm.23 = tail call dereferenceable_or_null(8) i8* @malloc(i32 8)
  %vtpm.24 = bitcast i8* %vtpm.23 to %Main*
  %vtpm.25 = icmp eq i8* %vtpm.23, null
  br i1 %vtpm.25, label %abort, label %ok.0

ok.0:                                             ; preds = %entry
  %vtpm.26 = getelementptr %Main, %Main* %vtpm.24, i64 0, i32 0
  store %_Main_vtable* @_Main_vtable_prototype, %_Main_vtable** %vtpm.26, align 8
  ret %Main* %vtpm.24

abort:                                            ; preds = %entry
  tail call void @abort()
  unreachable
}

attributes #0 = { inaccessiblememonly mustprogress nofree nounwind willreturn }
