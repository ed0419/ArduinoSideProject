using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Net.Http.Headers;
using System.Web.Http;
using System.Data.SqlClient;
using System.Data;
using Newtonsoft.Json.Linq;

namespace door.Controllers
{

    public class ValuesController : ApiController
    {
        [HttpGet]
        public HttpResponseMessage Get()
        {
            var response = new HttpResponseMessage();
            response.Content = new StringContent("<h1>OK阿!沒有問題! >< </h1>", System.Text.Encoding.GetEncoding("BIG5"));
            response.Content.Headers.ContentType = new MediaTypeHeaderValue("text/html");
            return response;
        }

        [HttpPost]
        public string Time()
        {
            return "現在時間" + DateTime.Now.ToString();
        }
        public class input
        {
            public String api_key;
            public String door_id;
            public String stu_id;
        }

        public object AddStuEntry(dynamic inputData)
        {

            var response = new
            {
                Status = "Not Vaild API_KEY",
                ID = ""
            };
            var Newbie = Convert.ToString(inputData);
            if (Newbie.Length == 6)
            {
                SqlConnection sqlc = new SqlConnection("Data Source=gg60.mc2021.net;Persist Security Info=True;Initial Catalog = entryLog;User ID=sa;Password=ovh1024768@");
                sqlc.Open();
                SqlCommand query = new SqlCommand("insert into logs(STU_ID,TIME,DOOR_ID) values(@P0,@P1,@P2)", sqlc);
                query.Parameters.AddWithValue("@P0", inputData.ToString());
                DateTimeOffset timestamp = DateTimeOffset.Now;
                query.Parameters.AddWithValue("@P1", timestamp);
                query.Parameters.AddWithValue("@P2", "A01");
                query.ExecuteNonQuery();
                response = new
                {
                    Status = "OK Got Your ID",
                    ID = "" + timestamp
                };
            }
            else
            {
                JObject j_data = JObject.Parse(Newbie);
                if (j_data.GetValue("api_key").ToString() == "thisisatestkey")
                {
                    SqlConnection sqlc = new SqlConnection("Data Source=gg60.mc2021.net;Persist Security Info=True;Initial Catalog = entryLog;User ID=sa;Password=ovh1024768@");
                    sqlc.Open();
                    SqlCommand query = new SqlCommand("insert into logs(STU_ID,TIME,DOOR_ID) values(@P0,@P1,@P2)", sqlc);
                    query.Parameters.AddWithValue("@P0", j_data.GetValue("stu_id").ToString());
                    DateTimeOffset timestamp = DateTimeOffset.Now;
                    query.Parameters.AddWithValue("@P1", timestamp);
                    query.Parameters.AddWithValue("@P2", j_data.GetValue("door_id").ToString());
                    query.ExecuteNonQuery();
                    response = new
                    {
                        Status = "OK Got Your ID",
                        ID = "" + timestamp
                    };
                }
                else
                {
                    response = new
                    {
                        Status = "Not Vaild API_KEY",
                        ID = ""
                    };
                }
            }

            return response;
        }
    }
}