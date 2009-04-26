class AddTermsToUsers < ActiveRecord::Migration
  def self.up
    add_column :users, :terms, :boolean, :default => nil
  end

  def self.down
    remove_column :users, :terms
  end
end
