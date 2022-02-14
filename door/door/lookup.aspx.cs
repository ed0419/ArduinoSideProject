using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Data.SqlClient;
using System.Data;

namespace door
{
    public partial class lookup : System.Web.UI.Page
    {
        protected override void OnPreRender(EventArgs e)
        {
            base.OnPreRender(e);
            SqlConnection sqlc = new SqlConnection("Data Source=gg60.mc2021.net;Persist Security Info=True;Initial Catalog = entryLog;User ID=doorMgmt;Password=Ovh1024768@");
            sqlc.Open();
            string sqltext2 = "SELECT logs.ID,students.NAME,logs.STU_ID,TIME,DOOR_ID FROM logs INNER JOIN students ON logs.STU_ID=students.STU_ID";
            SqlDataAdapter ad = new SqlDataAdapter(sqltext2, sqlc);
            DataTable dt = new DataTable();

            ad.Fill(dt);
            GridView1.DataSource = dt;
            GridView1.DataBind();
        }
        protected void Page_Load(object sender, EventArgs e)
        {

        }
        protected void GridView1_RowCommand(object sender, GridViewCommandEventArgs e)
        {
            if (e.CommandName == "刪除")
            {
                SqlConnection sqlc = new SqlConnection("Data Source=gg60.mc2021.net;Persist Security Info=True;Initial Catalog = entryLog;User ID=sa;Password=ovh1024768@");
                sqlc.Open();
                Button BTN = (Button)e.CommandSource;
                GridViewRow myRow = (GridViewRow)BTN.NamingContainer;
                int pos = myRow.RowIndex;
                string key = (sender as GridView).DataKeys[pos].Value.ToString();
                string sqltext = "delete from logs where ID = @P0";

                SqlCommand query = new SqlCommand(sqltext, sqlc);
                query.Parameters.AddWithValue("@P0", key);
                query.ExecuteNonQuery();
                //Label1.Text = pos.ToString() + key;
            }
        }


    }
}