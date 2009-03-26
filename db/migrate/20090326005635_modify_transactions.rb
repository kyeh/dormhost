class ModifyTransactions < ActiveRecord::Migration
  def self.up
    add_column :transactions, :approved, :boolean
  end

  def self.down
    remove column :transactions, :approved
  end
end
