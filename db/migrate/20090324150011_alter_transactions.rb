class AlterTransactions < ActiveRecord::Migration
  def self.up
    add_column :transactions, :room_id, :integer
    remove_column :transactions, :host_id
  end

  def self.down
    remove_column :transactions, :room_id
  end
end
