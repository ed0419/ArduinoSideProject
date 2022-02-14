<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="lookup.aspx.cs" Inherits="door.lookup" %>

<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
<link href="https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-1BmE4kWBq78iYhFldvKuhfTAU6auU8tT94WrHftjDbrCEXSU1oBoqyl2QvZ6jIW3" crossorigin="anonymous">
<script src="https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/js/bootstrap.bundle.min.js" integrity="sha384-ka7Sk0Gln4gmtz2MlQnikT1wXgYsOg+OMhuP+IlRH9sENBO0LRn5q+8nbTov4+1p" crossorigin="anonymous"></script>
    <title></title>
</head>
<body>
    <form id="form1" runat="server">
        <div class="container">
            <asp:GridView ID="GridView1" class="table table-info table-bordered table-condensed table-responsive table table-striped caption-top" runat="server" AutoGenerateColumns="False" AllowSorting="True" DataKeyNames="ID" OnRowCommand="GridView1_RowCommand" ShowHeaderWhenEmpty="True" ViewStateMode="Enabled">
                <Columns>
                    <asp:BoundField DataField="ID" HeaderText="UID" NullDisplayText="wdwdasda" />
                    <asp:BoundField DataField="NAME" HeaderText="學生姓名" />
                    <asp:BoundField DataField="STU_ID" HeaderText="學號" />
                    <asp:BoundField DataField="TIME" HeaderText="登記時間" />
                    <asp:BoundField DataField="DOOR_ID" HeaderText="消毒門編號" />
                    <asp:TemplateField HeaderText="操作">
                        <ItemTemplate>
                            <asp:Button ID="bt_delete" runat="server" Text="刪除紀錄" CommandName="刪除" />
                        </ItemTemplate>
                    </asp:TemplateField>
                </Columns>
            </asp:GridView>
        </div>
    </form>
</body>
</html>
