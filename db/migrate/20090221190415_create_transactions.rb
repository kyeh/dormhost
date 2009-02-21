class CreateTransactions < ActiveRecord::Migration
  def self.up
    create_table :transactions do |t|
      t.column  :host_id,     :integer, :null => false
      t.column  :renter_id,   :integer, :null => false
      t.column  :created_at,  :timestamp
      t.column  :active,      :boolean, :default => 0

      t.timestamps
    end
  end

  def self.down
    drop_table :transactions
  end
end
