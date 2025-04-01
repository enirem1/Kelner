using System;
using System.Collections.Generic;
using System.ComponentModel;  // For BindingList
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Net.Http.Headers;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Newtonsoft.Json;

namespace Kelnercho
{

    public partial class Form1 : Form
    {

        int row = 0;
       
        static HttpClient client = new HttpClient();
        private BindingList<Order> orders;  // Use BindingList for change notifications
        private Timer refreshTimer;  // Timer to refresh orders periodically

        public Form1()
        {
            InitializeComponent();
            InitializeDataGridView();
            client.BaseAddress = new Uri("http://localhost:5000/");
            client.DefaultRequestHeaders.Accept.Clear();
            client.DefaultRequestHeaders.Accept.Add(new MediaTypeWithQualityHeaderValue("application/json"));
            orders = new BindingList<Order>();  
            dataGridView1.DataSource = orders;  
       
            refreshTimer = new Timer();
            refreshTimer.Interval = 1000; 
            refreshTimer.Tick += RefreshTimer_Tick;
            refreshTimer.Start();

            LoadOrdersAsync();  
            
        }

        private async void RefreshTimer_Tick(object sender, EventArgs e)
        {
            await LoadOrdersAsync();
            
            if (dataGridView1.Rows.Count != 0)
            {
               
                SelectRow(row);// Reload the orders from the server
            }

        }
        private void InitializeDataGridView()
        {
            dataGridView1.AutoGenerateColumns = false;
            dataGridView1.Columns.Clear();

            dataGridView1.Columns.Add(new DataGridViewTextBoxColumn
            {
                DataPropertyName = "OrderId",
                HeaderText = "Order ID",
                Name = "OrderId"
            });

            dataGridView1.Columns.Add(new DataGridViewTextBoxColumn
            {
                DataPropertyName = "TableId",
                HeaderText = "Table ID",
                Name = "TableId"
            });

            dataGridView1.Columns.Add(new DataGridViewTextBoxColumn
            {
                DataPropertyName = "Name",
                HeaderText = "Name",
                Name = "Name"
            });

            dataGridView1.Columns.Add(new DataGridViewTextBoxColumn
            {
                DataPropertyName = "Weight",
                HeaderText = "Weight",
                Name = "Weight"
            });

            dataGridView1.Columns.Add(new DataGridViewTextBoxColumn
            {
                DataPropertyName = "Price",
                HeaderText = "Price",
                Name = "Price"
            });

            dataGridView1.DataSource = orders;
        }

        private async Task LoadOrdersAsync()
        {
            try
            {
                // Save the current scroll position
                int firstDisplayedScrollingRowIndex = dataGridView1.FirstDisplayedScrollingRowIndex;

                // Suspend layout to prevent flickering
                dataGridView1.SuspendLayout();

                var response = await client.GetAsync("/Show/Orders");
                if (response.IsSuccessStatusCode)
                {
                    var ordersJson = await response.Content.ReadAsStringAsync();
                    var ordersList = JsonConvert.DeserializeObject<List<Order>>(ordersJson);

                    // Handle null OrderId values (optional, depending on your requirements)
                    foreach (var order in ordersList)
                    {
                        if (order.OrderId == null)
                        {
                            // Assign a default value or skip the order
                            order.OrderId = 0; // Example: Assign 0 for null OrderId
                        }
                    }

                    // Update the BindingList
                    orders.Clear();
                    foreach (var order in ordersList)
                    {
                        orders.Add(order);
                    }

                    // Restore the scroll position
                    if (firstDisplayedScrollingRowIndex >= 0 && firstDisplayedScrollingRowIndex < dataGridView1.Rows.Count)
                    {
                        dataGridView1.FirstDisplayedScrollingRowIndex = firstDisplayedScrollingRowIndex;
                    }
                }
                else
                {
                    MessageBox.Show("Error fetching orders. Status Code: " + response.StatusCode, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("An error occurred: " + ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            finally
            {
                // Resume layout
                dataGridView1.ResumeLayout();
            }
        }
        private async void Delete_Click(object sender, EventArgs e)
        {
            foreach (DataGridViewRow item in dataGridView1.SelectedRows)
            {
                try
                {
                    int? orderId = item.Cells["OrderId"].Value as int?; // Handle nullable OrderId
                    if (orderId.HasValue)
                    {
                        // Debug: Show the OrderId being deleted
                        MessageBox.Show($"Deleting Order ID: {orderId.Value}");

                        // Send DELETE request to the API
                        HttpResponseMessage response = await client.DeleteAsync($"/tables/order/{orderId.Value}");

                        // Debug: Show the API response
                        var responseContent = await response.Content.ReadAsStringAsync();
                        MessageBox.Show($"API Response: {responseContent}");

                        // Ensure the request was successful
                        response.EnsureSuccessStatusCode();

                        // Remove the order from the BindingList
                        Order orderToRemove = orders.FirstOrDefault(order => order.OrderId == orderId.Value);
                        if (orderToRemove != null)
                        {
                            orders.Remove(orderToRemove);
                        }
                    }
                    else
                    {
                        MessageBox.Show("Order ID is null. Cannot delete this order.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                }
                catch (HttpRequestException ex)
                {
                    MessageBox.Show($"HTTP error deleting order: {ex.Message}");
                }
                catch (Exception ex)
                {
                    MessageBox.Show($"Error deleting order: {ex.Message}");
                }
            }
        }

        private async void DeleteAll_Click(object sender, EventArgs e)
        {           
                //// Confirmation before deletion
                //var confirmResult = MessageBox.Show(
                //    "Are you sure you want to delete all orders?",
                //    "Confirm Delete All",
                //    MessageBoxButtons.YesNo,
                //    MessageBoxIcon.Warning
                //);

                //if (confirmResult == DialogResult.Yes)
                //{
                    try
                    {
                        // Loop through all orders and delete each from the server
                        foreach (var order in orders.ToList()) // ToList to avoid modifying collection while iterating
                        {
                            HttpResponseMessage response = await client.DeleteAsync($"/tables/order/{order.OrderId}");
                            response.EnsureSuccessStatusCode();
                        }

                        // Clear the BindingList to remove all items from the DataGridView
                        orders.Clear();
                       // MessageBox.Show("All orders have been deleted.", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show($"Error deleting all orders: {ex.Message}", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
               // }
            

        }

        private void Up_Click(object sender, EventArgs e)
        {
            if (dataGridView1.Rows.Count != 0)
            {
                row--;
                if (row < 0)
                {
                    row = dataGridView1.Rows.Count-1;
                }
                SelectRow(row);
            }          
        }

        private void Down_Click(object sender, EventArgs e)
        {
            if (dataGridView1.Rows.Count != 0)
            {
                row++;
                if (row >= dataGridView1.Rows.Count)
                {
                    row = 0;
                }
                SelectRow(row);

            }
        }

            private void SelectRow(int rowIndex)
        {
            dataGridView1.ClearSelection();
            dataGridView1.Rows[rowIndex].Selected = true;
        }

        private void dataGridView1_CellClick(object sender, DataGridViewCellEventArgs e)
        {
            //MessageBox.Show(e.ColumnIndex.ToString());
            if (e.RowIndex >= 0)
            {
                row = e.RowIndex;
            }
            else if (e.ColumnIndex == 0)
            {
                SortOrdersByOrderId();

            }

        }

        private void dataGridView1_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {
            if (e.RowIndex >= 0)
            {
                row = e.RowIndex;
            }
        }

        private void dataGridView1_CellDoubleClick(object sender, DataGridViewCellEventArgs e)
        {

        }
        private void dataGridView1_CellMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {

        }

        // Timer event to reload orders every 10 seconds
        //private void timer1_Tick(object sender, EventArgs e)
        //{
        //    LoadOrdersAsync();
        //    SelectRow(row);
        //}

        // Manually trigger a refresh after a new order is added
        public void AddNewOrder(Order newOrder)
        {
            orders.Add(newOrder);  // Adds new order to the BindingList
        }
        private void SortOrdersByOrderId()
        {
            // Sort orders by OrderId and update the BindingList
            var sortedOrders = orders.OrderByDescending(o => o.OrderId).ToList();
            orders = new BindingList<Order>(sortedOrders);
            dataGridView1.DataSource = orders;
        }



    }

    // Order model to map JSON data from API
    public class Order
    {
        public int? OrderId { get; set; } // Nullable int
        public int? TableId { get; set; } // Nullable int
        public string Name { get; set; }
        public string Price { get; set; } // Keep as string if the API returns it as a string
        public int Weight { get; set; }
    }
}
